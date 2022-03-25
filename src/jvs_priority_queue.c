#include <jvs_core.h>

int jvs_pq_init(jvs_pq_t *jvs_pq, jvs_pq_compator_ptr comp, size_t cap) {
    jvs_pq->pq = (void **)malloc(sizeof(void *) * cap);
    if (!jvs_pq->pq) {
        return JVS_ERROR;
    }
    jvs_pq->cap = cap;
    jvs_pq->size = 0;
    jvs_pq->comp = comp;
    return JVS_OK;
}

int jvs_pq_is_empty(jvs_pq_t *jvs_pq) {
    return jvs_pq->size == 0 ? 1 : 0;
}

size_t jvs_pq_size(jvs_pq_t *jvs_pq) {
    return jvs_pq->size;
}

void *jvs_pq_min(jvs_pq_t *jvs_pq) {
    if (jvs_pq_is_empty(jvs_pq)) {
        return (void *)(-1);
    }
    return jvs_pq->pq[0];
}

static void swap(jvs_pq_t *jvs_pq, size_t i, size_t j){
    void *tmp = jvs_pq->pq[i];
    jvs_pq->pq[i] = jvs_pq->pq[j];
    jvs_pq->pq[j] = tmp;
}

size_t parent_index(size_t son_index) {
    return son_index % 2 == 0 ? ((son_index - 2) / 2) : ((son_index - 1) / 2);
}

void adjust_up(jvs_pq_t *jvs_pq, size_t k){
    if (k == 0) {
        return;
    }
    size_t parent = parent_index(k);
    while (jvs_pq->comp(jvs_pq->pq[k], jvs_pq->pq[parent])){
        swap(jvs_pq, k, parent);
        k = parent;
        parent = parent_index(k);
    }
}


static void adjust_down(jvs_pq_t *jvs_pq, size_t k){
    size_t j;
    size_t size = jvs_pq->size;
    while((k * 2 + 2) < size){
        j = k * 2 + 1;
        if((j < size) && (jvs_pq->comp(jvs_pq->pq[j+1], jvs_pq->pq[j])))
            j++;

        if(!jvs_pq->comp(jvs_pq->pq[j], jvs_pq->pq[k]))
            break;

        swap(jvs_pq, j, k);
        k = j;
    }
}

int jvs_pq_resize(jvs_pq_t *jvs_pq) {
    size_t new_cap = sizeof(jvs_pq->cap * 2);
    void **new_pq = (void **)malloc(sizeof(void *) * new_cap);
    if (!new_pq) {
        return JVS_ERROR;
    }
    memcpy(new_pq, jvs_pq, jvs_pq->size);
    free(jvs_pq->pq);
    jvs_pq->pq = new_pq;
    jvs_pq->cap = new_cap;
    return JVS_OK;
}

int jvs_pq_delmin(jvs_pq_t *jvs_pq) {
    if (jvs_pq_is_empty(jvs_pq)) {
        return JVS_OK;
    }
    --jvs_pq->size;
    swap(jvs_pq, 0, jvs_pq->size);
    free(jvs_pq->pq[jvs_pq->size]);
    jvs_pq->pq[jvs_pq->size] = NULL;
    adjust_down(jvs_pq, 0);
    return JVS_OK;
}

int jvs_pq_insert(jvs_pq_t *jvs_pq, void *item) {
    if (jvs_pq->size > jvs_pq->cap * 0.75) {
        if (jvs_pq_resize(jvs_pq) == JVS_ERROR) {
            return JVS_ERROR;
        }
    }

    jvs_pq->pq[jvs_pq->size] = item;
    adjust_up(jvs_pq, jvs_pq->size);
    jvs_pq->size++;
    return JVS_OK;
}


