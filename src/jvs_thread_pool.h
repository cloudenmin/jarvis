#ifndef _JVS_THREAD_POOL_INCLUDED_
#define _JVS_THREAD_POOL_INCLUDED_

#include <jvs_core.h>

#define THREAD_NUM 20

struct jvs_thread_task {
    struct jvs_thread_task     *next;
    int                        id;
    void                       *ctx;
    void                       (*handler)(void *conn);
};

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

int jvs_thread_pool_init(jvs_thread_pool_t *tp);
jvs_thread_task_t *jvs_thread_task_alloc(void *conn);
int jvs_thread_pool_task_add(jvs_thread_pool_t *tp, jvs_thread_task_t *task);

#endif /* _JVS_THREAD_POOL_INCLUDED_ */