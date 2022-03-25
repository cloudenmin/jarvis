
#include <jvs_core.h>

static int ep = -1;

static struct epoll_event event_list[MAX_EVENT] = {};

int jvs_epoll_init() {
    if (ep == -1) {
        ep = epoll_create(MAX_EVENT);
        if (ep == -1) {
            return JVS_ERROR;
        }
    }
    printf("create epoll fd: %d\n", ep);
    return JVS_OK;
}

int jvs_epoll_add(int fd, uint32_t events, jvs_http_connect_t *conn) {
    struct epoll_event ev = {0};

    if(fd < 0) return JVS_ERROR;

    ev.events = events;
    ev.data.ptr = (void *) conn;
    printf("add listen fd %d to epoll fd %d, event is %d\n", conn->fd, ep, ev.events);
    printf("epoll event %p\n", &ev);
    return epoll_ctl(ep, EPOLL_CTL_ADD, fd, &ev) == 0 ? JVS_OK : JVS_ERROR;
}

int jvs_epoll_mod(int fd, uint32_t events, jvs_http_connect_t *conn) {
    struct epoll_event ev = {0};

    if(fd < 0) return JVS_ERROR;
    ev.data.ptr = (void *)conn;
    ev.events = events;
    return epoll_ctl(ep, EPOLL_CTL_MOD, fd, &ev) == 0 ? JVS_OK : JVS_ERROR;
}

int jvs_epoll_del(int fd) {
    struct epoll_event ev = {0};
    if(fd < 0) return JVS_ERROR;
    return epoll_ctl(ep, EPOLL_CTL_DEL, fd, &ev) == 0 ? JVS_OK : JVS_ERROR;
}

int jvs_epoll_wait(int timeout) {
    return epoll_wait(ep, event_list, MAX_EVENT, timeout);
}

void *jvs_get_event_conn(size_t i) {
    return event_list[i].data.ptr;
}

uint32_t jvs_get_events(size_t i) {
    return event_list[i].events;
}

jvs_epoll_event jvs_get_epoll_event(size_t i) {
    return event_list[i];
}

