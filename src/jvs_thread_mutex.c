#include <jvs_core.h>

int jvs_thread_mutex_create(pthread_mutex_t *mtx) {
    int                  err;
    pthread_mutexattr_t  attr;

    err = pthread_mutexattr_init(&attr);
    if (err != 0) {
        return JVS_ERROR;
    }

    err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    if (err != 0) {
        return JVS_ERROR;
    }

    err = pthread_mutex_init(mtx, &attr);
    if (err != 0) {
        return JVS_ERROR;
    }

    pthread_mutexattr_destroy(&attr);
    return JVS_OK;
}

int jvs_thread_mutex_destroy(pthread_mutex_t *mtx) {
    int err;

    err = pthread_mutex_destroy(mtx);
    if (err != 0) {
        return JVS_ERROR;
    }
    return JVS_OK;
}

int jvs_thread_mutex_lock(pthread_mutex_t *mtx) {
    int err;
    
    err = pthread_mutex_lock(mtx);
    if (err == 0) {
        return JVS_OK;
    }
    return JVS_ERROR;
}

int jvs_thread_mutex_unlock(pthread_mutex_t *mtx) {
    int err;

    err = pthread_mutex_unlock(mtx);
    if (err == 0) {
        return JVS_OK;
    }

    return JVS_ERROR;
}
