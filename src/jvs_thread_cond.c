#include <jvs_core.h>

int jvs_thread_cond_create(pthread_cond_t *cond) {
    int err;
    
    err = pthread_cond_init(cond, NULL);
    if (err == 0) {
        return JVS_OK;
    }
    return JVS_ERROR;
}

int jvs_thread_cond_destroy(pthread_cond_t *cond) {
    int err;

    err = pthread_cond_destroy(cond);
    if (err == 0) {
        return JVS_OK;
    }
    return JVS_ERROR;
}

int jvs_thread_cond_signal(pthread_cond_t *cond) {
    int err;

    err = pthread_cond_signal(cond);
    if (err == 0) {
        return JVS_OK;
    }
    return JVS_ERROR;
}

int jvs_thread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mtx) {
    int err;

    err = pthread_cond_wait(cond, mtx);
    if (err == 0) {
        return JVS_OK;
    }
    return JVS_ERROR;
}