#include <jvs_core.h>

jvs_pq_t jvs_timer;
size_t jvs_current_msec;

void jvs_time_update(){
    struct timeval tv;
    int rc = gettimeofday(&tv, NULL);
    jvs_current_msec = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int jvs_timer_init() {
    jvs_pq_init(&jvs_timer, timer_comp, JVS_PQ_DEFAULT_SIZE);
    jvs_time_update();
    return JVS_OK;
}

int jvs_timer_find()
{
    int time;
    while (!jvs_pq_is_empty(&jvs_timer)) {
        jvs_time_update();
        jvs_timer_t *timer_node = (jvs_timer_t *) jvs_pq_min(&jvs_timer);
        if(timer_node->deleted) {
            int rc = jvs_pq_delmin(&jvs_timer);
            continue;
        }
        time = (int)(timer_node->key - jvs_current_msec);
        time = (time > 0) ? time : 0;
        break;
    }
    return time;
}

void jvs_timer_add(jvs_http_connect_t *connect, size_t timeout, timer_handler_pt handler)
{
    jvs_time_update();
    jvs_timer_t *timer_node = (jvs_timer_t *)malloc(sizeof(jvs_timer_t));
    connect->timer = timer_node;
    timer_node->connect = connect;
    timer_node->key = jvs_current_msec + timeout;
    timer_node->handler = handler;
    jvs_pq_insert(&jvs_timer, timer_node);
}

void jvs_timer_del(jvs_http_connect_t *connect)
{
    jvs_time_update();
    jvs_timer_t *timer_node = connect->timer;
    timer_node->deleted = 1;
}

void jvs_handle_expire_timers()
{
    while (!jvs_pq_is_empty(&jvs_timer)) {
        jvs_time_update();
        jvs_timer_t *timer_node = (jvs_timer_t *)jvs_pq_min(&jvs_timer);
        if (timer_node->deleted) {
            jvs_pq_delmin(&jvs_timer);
            // free(timer_node);
            continue;
        }
        if(timer_node->key > jvs_current_msec) {
            return;
        }
        if(timer_node->handler) {
            timer_node->handler(timer_node->connect);
        }
        jvs_pq_delmin(&jvs_timer);
        // free(timer_node);
    }
}

int timer_comp(void *ti, void *tj) {
    jvs_timer_t *pi_t = (jvs_timer_t *) ti;
    jvs_timer_t *pj_t = (jvs_timer_t *) tj;
    return (pi_t->key < pj_t->key) ? 1 : 0;
}
