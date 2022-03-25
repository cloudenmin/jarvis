#ifndef _JVS_TIMER_H_INCLUDED_
#define _JVS_TIMER_H_INCLUDED_

#include <jvs_core.h>

typedef int (*timer_handler_pt)(jvs_http_connect_t* connect);

struct jvs_timer{
    size_t              key;
    int                 deleted;
    timer_handler_pt    handler;
    jvs_http_connect_t* connect;
};

extern jvs_pq_t jvs_timer;
extern size_t jvs_current_msec;

int jvs_timer_init();
int jvs_timer_find();
void jvs_timer_add(jvs_http_connect_t* connect, size_t timeout, timer_handler_pt handler);
void jvs_timer_del(jvs_http_connect_t* connect);
void jvs_handle_expire_timers();
int timer_comp(void *ti, void *tj);

#endif