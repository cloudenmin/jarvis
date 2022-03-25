#include <jvs_core.h>

#define MAX_FD 65535
#define PORT 8080

struct jvs_cycle_s {
    int port;

    jvs_thread_pool_t *tp;

    int listenfd;

    int thread_num;

    int OPT_LINGER;
};

typedef struct jvs_cycle_s jvs_cycle_t;

int jvs_event_listen(jvs_cycle_t *cycle);
int jvs_event_loop(jvs_cycle_t *cycle);
int jvs_cycle_init(jvs_cycle_t *cycle);


int main(int argc, char *const *argv) {
    jvs_cycle_t cycle;
    jvs_cycle_init(&cycle);
    jvs_epoll_init();
    jvs_thread_pool_init(cycle.tp);
    jvs_event_listen(&cycle);
    jvs_timer_init();
    jvs_event_loop(&cycle);
    sleep(100000);
    return JVS_OK;
}


int jvs_event_listen(jvs_cycle_t *cycle) {
    cycle->listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cycle->listenfd < 0) {
        return JVS_ERROR;
    }
    
    struct sockaddr_in address;
    bzero(&address, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    address.sin_port = htons(cycle->port);

    int flag = 1;
    setsockopt(cycle->listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    printf("start to bind port-%d into fd-%d to be listened\n", cycle->port, cycle->listenfd);
    if (bind(cycle->listenfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        return JVS_ERROR;
    }
    
    printf("start to listen ip: %s, port: %d\n", "127.0.0.1", cycle->port);
    if (listen(cycle->listenfd, 1024) < 0) {
        return JVS_ERROR;
    }

    jvs_http_connect_t *conn = jvs_http_conn_new(cycle->listenfd, &address);
    int result = jvs_epoll_add(cycle->listenfd, (EPOLLIN | EPOLLET), conn);

    if(result != 0) {
        printf("failed to add listen fd-%d to epoll\n", cycle->listenfd);
    }
    printf("epoll add success\n");

    set_fd_nonblock(cycle->listenfd);
}


int 
jvs_event_loop(jvs_cycle_t *cycle) {

    while (1)
    {
        int num = 0;
        printf("wait to request...\n");
        jvs_timer_find();
        num = jvs_epoll_wait(-1);
        if (num < 0) {
            break;
        }

        jvs_handle_expire_timers();

        for (int i = 0; i < num; i++) {
            jvs_http_connect_t *conn = (jvs_http_connect_t *)jvs_get_event_conn(i);
            printf("get connection, fd-%d, conn-%p\n", conn->fd, conn);
            int fd = conn->fd;
            if(fd==0) {
                break;
            }
            if (fd == cycle->listenfd) {
                jvs_http_accept_connection(fd);
            } else {
                if ((jvs_get_events(i) & EPOLLERR) || (jvs_get_events(i) & EPOLLHUP)
                    || (!(jvs_get_events(i) & EPOLLIN))){
                    close(fd);
                    continue;
                }
                jvs_thread_task_t *task = jvs_thread_task_alloc(conn);
                jvs_thread_pool_task_add(cycle->tp, task);

                // jvs_http_do_request(conn);
            }
        }
    }   
}


int jvs_cycle_init(jvs_cycle_t *cycle) {
    printf("start to init cycle\n");
    cycle->port = PORT;
    cycle->tp = (jvs_thread_pool_t *)malloc(sizeof(jvs_thread_pool_t));
    cycle->thread_num = 8;
    cycle->tp->thread_num = cycle->thread_num;
    cycle->tp->max_queue = 100;
    cycle->OPT_LINGER = 0;
    printf("finish to init cycle\n");
    return JVS_OK;
}

void set_fd_nonblock(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}