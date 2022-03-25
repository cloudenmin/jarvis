#ifndef _JVS_HTTP_CONNECT_H_INCLUDED_
#define _JVS_HTTP_CONNECT_H_INCLUDED_

#include <jvs_core.h>

#define JVS_HTTP_MAX_URI_CHANGES           10
#define JVS_HTTP_MAX_SUBREQUESTS           50

#define JVS_HTTP_LC_HEADER_LEN             32


#define JVS_HTTP_DISCARD_BUFFER_SIZE       4096
#define JVS_HTTP_LINGERING_BUFFER_SIZE     4096


#define JVS_HTTP_VERSION_9                 9
#define JVS_HTTP_VERSION_10                1000
#define JVS_HTTP_VERSION_11                1001
#define JVS_HTTP_VERSION_20                2000

#define JVS_HTTP_UNKNOWN                   0x00000001
#define JVS_HTTP_GET                       0x00000002
#define JVS_HTTP_HEAD                      0x00000004
#define JVS_HTTP_POST                      0x00000008
#define JVS_HTTP_PUT                       0x00000010
#define JVS_HTTP_DELETE                    0x00000020
#define JVS_HTTP_MKCOL                     0x00000040
#define JVS_HTTP_COPY                      0x00000080
#define JVS_HTTP_MOVE                      0x00000100
#define JVS_HTTP_OPTIONS                   0x00000200
#define JVS_HTTP_PROPFIND                  0x00000400
#define JVS_HTTP_PROPPATCH                 0x00000800
#define JVS_HTTP_LOCK                      0x00001000
#define JVS_HTTP_UNLOCK                    0x00002000
#define JVS_HTTP_PATCH                     0x00004000
#define JVS_HTTP_TRACE                     0x00008000
#define JVS_HTTP_CONNECT                   0x00010000

#define JVS_HTTP_CONNECTION_CLOSE          1
#define JVS_HTTP_CONNECTION_KEEP_ALIVE     2


#define JVS_NONE                           1


#define JVS_HTTP_PARSE_HEADER_DONE         1

#define JVS_HTTP_CLIENT_ERROR              10
#define JVS_HTTP_PARSE_INVALID_METHOD      10
#define JVS_HTTP_PARSE_INVALID_REQUEST     11
#define JVS_HTTP_PARSE_INVALID_VERSION     12
#define JVS_HTTP_PARSE_INVALID_09_METHOD   13

#define JVS_HTTP_PARSE_INVALID_HEADER      14


#define JVS_HTTP_SUBREQUEST_IN_MEMORY      2
#define JVS_HTTP_SUBREQUEST_WAITED         4
#define JVS_HTTP_SUBREQUEST_CLONE          8
#define JVS_HTTP_SUBREQUEST_BACKGROUND     16

#define JVS_HTTP_LOG_UNSAFE                1


#define JVS_HTTP_CONTINUE                  100
#define JVS_HTTP_SWITCHING_PROTOCOLS       101
#define JVS_HTTP_PROCESSING                102

#define JVS_HTTP_OK                        200
#define JVS_HTTP_CREATED                   201
#define JVS_HTTP_ACCEPTED                  202
#define JVS_HTTP_NO_CONTENT                204
#define JVS_HTTP_PARTIAL_CONTENT           206

#define JVS_HTTP_SPECIAL_RESPONSE          300
#define JVS_HTTP_MOVED_PERMANENTLY         301
#define JVS_HTTP_MOVED_TEMPORARILY         302
#define JVS_HTTP_SEE_OTHER                 303
#define JVS_HTTP_NOT_MODIFIED              304
#define JVS_HTTP_TEMPORARY_REDIRECT        307
#define JVS_HTTP_PERMANENT_REDIRECT        308

#define JVS_HTTP_BAD_REQUEST               400
#define JVS_HTTP_UNAUTHORIZED              401
#define JVS_HTTP_FORBIDDEN                 403
#define JVS_HTTP_NOT_FOUND                 404
#define JVS_HTTP_NOT_ALLOWED               405
#define JVS_HTTP_REQUEST_TIME_OUT          408
#define JVS_HTTP_CONFLICT                  409
#define JVS_HTTP_LENGTH_REQUIRED           411
#define JVS_HTTP_PRECONDITION_FAILED       412
#define JVS_HTTP_REQUEST_ENTITY_TOO_LARGE  413
#define JVS_HTTP_REQUEST_URI_TOO_LARGE     414
#define JVS_HTTP_UNSUPPORTED_MEDIA_TYPE    415
#define JVS_HTTP_RANGE_NOT_SATISFIABLE     416
#define JVS_HTTP_MISDIRECTED_REQUEST       421
#define JVS_HTTP_TOO_MANY_REQUESTS         429


#define JVS_HTTP_CLOSE                     444

#define JVS_HTTP_NGINX_CODES               494

#define JVS_HTTP_REQUEST_HEADER_TOO_LARGE  494

#define JVS_HTTPS_CERT_ERROR               495
#define JVS_HTTPS_NO_CERT                  496


#define JVS_HTTP_TO_HTTPS                  497


#define JVS_HTTP_CLIENT_CLOSED_REQUEST     499


#define JVS_HTTP_INTERNAL_SERVER_ERROR     500
#define JVS_HTTP_NOT_IMPLEMENTED           501
#define JVS_HTTP_BAD_GATEWAY               502
#define JVS_HTTP_SERVICE_UNAVAILABLE       503
#define JVS_HTTP_GATEWAY_TIME_OUT          504
#define JVS_HTTP_VERSION_NOT_SUPPORTED     505
#define JVS_HTTP_INSUFFICIENT_STORAGE      507


#define JVS_HTTP_LOWLEVEL_BUFFERED         0xf0
#define JVS_HTTP_WRITE_BUFFERED            0x10
#define JVS_HTTP_GZIP_BUFFERED             0x20
#define JVS_HTTP_SSI_BUFFERED              0x01
#define JVS_HTTP_SUB_BUFFERED              0x02
#define JVS_HTTP_COPY_BUFFERED             0x04

#define JVS_READ_BUFFER_SIZE 8124
#define JVS_WRITE_BUFFER_SIZE 1024
#define JVS_FILENAME_LEN 200

#define JVS_CONN_TIME_OUT 5000000

struct jvs_http_connect {
    int           fd;
    jvs_str_t     remote_addr;
    int           remote_port;
    jvs_str_t     local_addr;
    int           local_port;
    jvs_timer_t   *timer;

    char          read_buff[JVS_READ_BUFFER_SIZE];
    size_t        pos;
    size_t        last;
};


jvs_http_connect_t *jvs_http_conn_new(int client_fd, struct sockaddr_in *client_addr);
void jvs_http_conn_destroy(jvs_http_connect_t *conn);
int jvs_http_accept_connection(int fd);
void jvs_http_do_request(void *conn);
#endif /* _JVS_HTTP_CONNECT_H_INCLUDED_ */