#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <syscall.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>

#define THREAD_NUM 10


struct jvs_thread_task {
    struct jvs_thread_task     *next;
    int                        id;
    int                        ctx;
    void                       (*handler)(int conn);
};

typedef struct jvs_thread_task jvs_thread_task_t;

typedef struct {
    jvs_thread_task_t  *first;
    jvs_thread_task_t  *last;
}jvs_thread_pool_queue_t;

struct jvs_thread_pool {
    pthread_mutex_t         mtx;
    jvs_thread_pool_queue_t queue;
    pthread_cond_t          cond;
    pthread_t               threads[THREAD_NUM];
    int                     thread_num;
    int                     waiting;
    int                     max_queue;
};

typedef struct jvs_thread_pool jvs_thread_pool_t;


static void *jvs_thread_pool_cycle(void *data) {
    jvs_thread_pool_t *tp = data;
    int      err;
    // sigset_t set;
    jvs_thread_task_t *task;

    // sigfillset(&set);

    // sigdelset(&set, SIGILL);
    // sigdelset(&set, SIGFPE);
    // sigdelset(&set, SIGSEGV);
    // sigdelset(&set, SIGBUS);

    // err = pthread_sigmask(SIG_BLOCK, &set, NULL);
    // if (err) {
    //     return NULL;
    // }
    for ( ;; ) {
        if (pthread_mutex_lock(&tp->mtx) != 0) {
            return NULL;
        }

        while (tp->queue.first == tp->queue.last) {
            if (pthread_cond_wait(&tp->cond, &tp->mtx) != 0) {
                (void) pthread_mutex_unlock(&tp->mtx);
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

        if (pthread_mutex_unlock(&tp->mtx) != 0) {
            return NULL;
        }
        task->handler(task->ctx);
        task->next = NULL;
        free(task);
    }
}

int jvs_thread_pool_init(jvs_thread_pool_t *tp) {
    printf("start to init thread pool\n");
    int err;
    pthread_t tid;
    int n;
    pthread_attr_t attr;

    if (pthread_mutex_init(&tp->mtx, NULL) != 0) {
        return -1;
    }

    if (pthread_cond_init(&tp->cond, NULL) != 0) {
        (void) pthread_mutex_destroy(&tp->mtx);
        return -1;
    }

    err = pthread_attr_init(&attr);
    if (err) {
        return -1;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err) {
        return -1;
    }

    for (n = 0; n < tp->thread_num; n++) {
        err = pthread_create(&(tp->threads[n]), &attr, jvs_thread_pool_cycle, tp);
        if (err) {
            return -1;
        }
    }

    (void) pthread_attr_destroy(&attr);
    printf("finish to init thread pool\n");
    return 0;
}


void handle(int conn) {
    printf("work %d\n", conn);
}

jvs_thread_task_t *jvs_thread_task_alloc(int conn) {
    jvs_thread_task_t *task = malloc(sizeof(jvs_thread_task_t));
    if (task == NULL) {
        return NULL;
    }
    task->handler = handle;
    task->ctx = conn;
    task->next = NULL;
    return task;
}

int jvs_thread_pool_task_add(jvs_thread_pool_t *tp, jvs_thread_task_t *task) {
    if (pthread_mutex_lock(&tp->mtx) != 0) {
        return -1;
    }

    if(tp->waiting >= tp->max_queue) {
        pthread_mutex_unlock(&tp->mtx);
        return -1;
    }

    task->id = 0;
    task->next = NULL;

    if (pthread_cond_signal(&tp->cond) != 0) {
        pthread_mutex_unlock(&tp->mtx);
        return -1;
    }

    tp->queue.last->next = task;
    tp->queue.last = task;
    tp->waiting++;

    (void) pthread_mutex_unlock(&tp->mtx);
    return 0;
}


int main() {
    jvs_thread_pool_t *tp = (jvs_thread_pool_t *)malloc(sizeof(jvs_thread_pool_t));
    tp->thread_num = THREAD_NUM;
    tp->waiting = tp->thread_num;
    tp->max_queue = 200;
    jvs_thread_task_t *head = (jvs_thread_task_t *)malloc(sizeof(jvs_thread_task_t));
    head->handler = NULL;
    head->ctx = 0;
    head->next = NULL;
    tp->queue.first = head;
    tp->queue.last = tp->queue.first;
    jvs_thread_pool_init(tp);
    for(int i=0; i<100; i++) {
        jvs_thread_task_t *task =  jvs_thread_task_alloc(i);
        jvs_thread_pool_task_add(tp, task);
    }
    printf("start to sleep...\n");
    sleep(5);
    free(tp);
}