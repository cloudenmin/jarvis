#ifndef _JVS_PRIORITY_QUEUE_INCLUDED_
#define _JVS_PRIORITY_QUEUE_INCLUDED_

#include <jvs_core.h>

#define JVS_PQ_DEFAULT_SIZE 10

typedef int (*jvs_pq_compator_ptr)(void *pi, void *pj);

struct jvs_priority_queue {
    void **pq;
    size_t cap;
    size_t size;
    jvs_pq_compator_ptr comp;
};

int jvs_pq_init(jvs_pq_t *jvs_pq, jvs_pq_compator_ptr comp, size_t size);
int jvs_pq_is_empty(jvs_pq_t *jvs_pq);
size_t jvs_pq_size(jvs_pq_t *jvs_pq);
void *jvs_pq_min(jvs_pq_t *jvs_pq);
int jvs_pq_delmin(jvs_pq_t *jvs_pq);
int jvs_pq_insert(jvs_pq_t *jvs_pq, void *item);
int jvs_pq_sink(jvs_pq_t *jvs_pq, size_t size_t);

#endif /* _JVS_PRIORITY_QUEUE_INCLUDED_ */