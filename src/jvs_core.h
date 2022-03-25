#ifndef _JVS_CORE_H_INCLUDED_
#define _JVS_CORE_H_INCLUDED_


#define  JVS_OK          0
#define  JVS_ERROR      -1
#define  JVS_AGAIN      -2
#define  JVS_BUSY       -3
#define  JVS_DONE       -4
#define  JVS_DECLINED   -5
#define  JVS_ABORT      -6

#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"

#define jvs_memzero(buf, n)       (void) memset(buf, 0, n)

#define jvs_abs(value)       (((value) >= 0) ? (value) : - (value))
#define jvs_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define jvs_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

typedef struct jvs_http_connect   jvs_http_connect_t;
typedef struct jvs_http_request   jvs_http_request_t;
typedef struct jvs_buffer         jvs_buffer_t;
typedef struct jvs_hash           jvs_hash_t;
typedef struct jvs_entry          jvs_entry_t;
typedef struct jvs_priority_queue jvs_pq_t;
typedef struct jvs_string         jvs_str_t;
typedef struct jvs_timer          jvs_timer_t;
typedef struct jvs_thread_task    jvs_thread_task_t;
typedef struct jvs_thread_pool    jvs_thread_pool_t;

#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syscall.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include <errno.h>

#include <jvs_thread_pool.h>
#include <jarvis.h>
#include <jvs_thread.h>
#include <jvs_priority_queue.h>
#include <jvs_timer.h>
#include <jvs_string.h>
//#include <jvs_rbtree.h>
#include <jvs_hash.h>
#include <jvs_http_connect.h>
#include <jvs_epoller.h>
#include <jvs_http_request.h>
#include <jvs_buffer_io.h>




#endif /* _JVS_CORE_H_INCLUDED_ */
