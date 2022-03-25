#ifndef _JVS_EPOLLER_INCLUDED_
#define _JVS_EPOLLER_INCLUDED_

#include <jvs_core.h>

#define MAX_EVENT 50

typedef struct epoll_event jvs_epoll_event;

int jvs_epoll_init();
int jvs_epoll_add(int fd, uint32_t events, jvs_http_connect_t *conn);
int jvs_epoll_del(int fd);
int jvs_epoll_wait(int timeout);
int jvs_epoll_mod(int fd, uint32_t events, jvs_http_connect_t *conn);
void *jvs_get_event_conn(size_t i);
uint32_t jvs_get_events(size_t i);
jvs_epoll_event jvs_get_epoll_event(size_t i);

#endif /* _JVS_EPOLLER_INCLUDED_ */