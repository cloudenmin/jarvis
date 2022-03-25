#include <jvs_core.h>

jvs_http_connect_t *jvs_http_conn_new(int client_fd, struct sockaddr_in *client_addr) {
    jvs_http_connect_t *conn = (jvs_http_connect_t *)malloc(sizeof(jvs_http_connect_t));
    bzero(conn, sizeof(jvs_http_connect_t));
    conn->fd = client_fd;
    inet_ntop(client_addr->sin_family, (void *)&(client_addr->sin_addr), conn->remote_addr.data, conn->remote_addr.size);
    conn->remote_port = ntohs(client_addr->sin_port);
    conn->last = 0;
    conn->pos = 0;
    return conn;
}

void jvs_http_conn_destroy(jvs_http_connect_t *conn) {
    if(conn == NULL) {
        return;
    }
    if(conn->remote_addr.data != NULL) {
        free(conn->remote_addr.data);
    }
    if(conn->local_addr.data != NULL) {
        free(conn->local_addr.data);
    }
    memset(conn->read_buff, 0, JVS_READ_BUFFER_SIZE);
}

int jvs_http_close_conn(jvs_http_connect_t *conn){
    close(conn->fd);
    jvs_http_conn_destroy(conn);
    return 0;
}

int jvs_http_accept_connection(int listen_fd)
{
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = 0;
    int accept_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(accept_fd == -1) {
        return JVS_ERROR;
    }
    set_fd_nonblock(accept_fd);
    jvs_http_connect_t *conn = jvs_http_conn_new(accept_fd, &client_addr);
    printf("create connection... %p\n", conn);
    jvs_epoll_add(accept_fd, EPOLLIN, conn);
    jvs_timer_add(conn, JVS_CONN_TIME_OUT, jvs_http_close_conn);
    return JVS_OK;
}

void jvs_http_do_request(void *conn)
{
    printf("start response...\n");
    jvs_http_connect_t* cur_conn = (jvs_http_connect_t*)conn;
    printf("conn info: %p\n", cur_conn);
    int fd = cur_conn->fd;
    struct stat sbuf;
    int rc, n_read;
    char* start_char = NULL;
    size_t remain_size;
    jvs_timer_del(cur_conn);

    while(1){
        start_char = &cur_conn->read_buff[cur_conn->last % JVS_READ_BUFFER_SIZE];
        remain_size = JVS_READ_BUFFER_SIZE - cur_conn->last % JVS_READ_BUFFER_SIZE;
        n_read = read(fd, start_char, remain_size);
        if(n_read == 0) {
            goto err;
        }
        if(n_read < 0 && (errno != EAGAIN)) {
             goto err;
        }
           
        if((n_read < 0) && (errno == EAGAIN)) {
            break;
        }

        cur_conn->last += n_read;
        jvs_http_request_t *cur_request = (jvs_http_request_t *)malloc(sizeof(jvs_http_request_t));
    
        jvs_http_parse_request(cur_conn, cur_request);
        u_char resp[] = "HTTP/1.1 200 OK\r\nContent-type: application/json\r\n\r\n{\"test\": \"test\"}";
        write(fd, resp, sizeof(resp));
        free(cur_request);
    }
    jvs_epoll_mod(fd, (EPOLLIN | EPOLLET | EPOLLONESHOT), cur_conn);
    jvs_timer_add(cur_conn, JVS_CONN_TIME_OUT, jvs_http_close_conn);

    err:
    close:
    jvs_http_close_conn(cur_conn);
}