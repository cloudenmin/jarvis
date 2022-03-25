#include <jvs_core.h>

static void jvs_thread_pool_queue_init(jvs_thread_pool_queue_t *queue)
{
    jvs_thread_task_t *head = (jvs_thread_task_t *)malloc(sizeof(jvs_thread_task_t)); 
    head->handler = NULL;
    head->ctx = NULL;
    head->next = NULL;
    queue->first = head;
    queue->last = queue->first;
}
    

static int jvs_thread_pool_task_id;

static void *jvs_thread_pool_cycle(void *data) {
    jvs_thread_pool_t *tp = data;
    int      err;
    jvs_thread_task_t *task;

    for ( ;; ) {
        if (jvs_thread_mutex_lock(&tp->mtx) != JVS_OK) {
            return NULL;
        }

        while (tp->queue.first == tp->queue.last) {
            if (jvs_thread_cond_wait(&tp->cond, &tp->mtx) != JVS_OK) {
                (void) jvs_thread_mutex_unlock(&tp->mtx);
                return NULL;
            }
        }

        task = tp->queue.first->next;
        tp->queue.first->next = task->next;

        if (task == tp->queue.last) {
            tp->queue.last = tp->queue.first;
        }
        
        tp->waiting--;
        task->next = NULL;

        if (jvs_thread_mutex_unlock(&tp->mtx) != JVS_OK) {
            return NULL;
        }
        task->handler(task->ctx);
        task->next = NULL;
    }
}

int jvs_thread_pool_init(jvs_thread_pool_t *tp) {
    printf("start to init thread pool\n");
    int err;
    int n;
    pthread_attr_t attr;

    jvs_thread_pool_queue_init(&tp->queue);
    if (jvs_thread_mutex_create(&tp->mtx) != JVS_OK) {
        return JVS_ERROR;
    }

    if (jvs_thread_cond_create(&tp->cond) != JVS_OK) {
        (void) jvs_thread_mutex_destroy(&tp->mtx);
        return JVS_ERROR;
    }

    err = pthread_attr_init(&attr);
    if (err) {
        return JVS_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err) {
        return JVS_ERROR;
    }

    for (n = 0; n < tp->thread_num; n++) {
        err = pthread_create(&(tp->threads[n]), &attr, jvs_thread_pool_cycle, tp);
        if (err) {
            return JVS_ERROR;
        }
    }

    (void) pthread_attr_destroy(&attr);
    printf("finish to init thread pool\n");
    return JVS_OK;
}

jvs_thread_task_t *jvs_thread_task_alloc(void *conn) {
    jvs_thread_task_t *task = malloc(sizeof(jvs_thread_task_t));
    if (task == NULL) {
        return NULL;
    }
    task->handler = jvs_http_do_request;
    task->ctx = conn;
    task->next = NULL;
    return task;
}

int jvs_thread_pool_task_add(jvs_thread_pool_t *tp, jvs_thread_task_t *task) {
    if (jvs_thread_mutex_lock(&tp->mtx) != JVS_OK) {
        return JVS_ERROR;
    }

    if(tp->waiting >= tp->max_queue) {
        (void) jvs_thread_mutex_unlock(&tp->mtx);
        return JVS_ERROR;
    }

    task->id = jvs_thread_pool_task_id++;
    task->next = NULL;

    if (jvs_thread_cond_signal(&tp->cond) != JVS_OK) {
        (void) jvs_thread_mutex_unlock(&tp->mtx);
        return JVS_ERROR;
    }

    tp->queue.last->next = task;
    tp->queue.last = task;
    tp->waiting++;

    (void) jvs_thread_mutex_unlock(&tp->mtx);
    return JVS_OK;
}