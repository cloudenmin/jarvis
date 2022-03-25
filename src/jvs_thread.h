#ifndef _JVS_THREAD_H_INCLUDED_
#define _JVS_THREAD_H_INCLUDED_

#include <jvs_core.h>

int jvs_thread_mutex_create(pthread_mutex_t *mtx);
int jvs_thread_mutex_destroy(pthread_mutex_t *mtx);
int jvs_thread_mutex_lock(pthread_mutex_t *mtx);
int jvs_thread_mutex_unlock(pthread_mutex_t *mtx);


int jvs_thread_cond_create(pthread_cond_t *cond);
int jvs_thread_cond_destroy(pthread_cond_t *cond);
int jvs_thread_cond_signal(pthread_cond_t *cond);
int jvs_thread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mtx);

#endif /* _JVS_THREAD_H_INCLUDED_ */