#ifndef _JVS_HTTP_REQUEST_H_INCLUDED_
#define _JVS_HTTP_REQUEST_H_INCLUDED_

#include <jvs_core.h>

struct jvs_http_request {
    jvs_str_t     uri;
    int           method;
    jvs_str_t     protocol;
    int           proto_major;
    int           proto_minor;

    jvs_hash_t    *param;
    jvs_hash_t    *header;
    u_char       *body;
};


int jvs_http_parse_request(jvs_http_connect_t *conn, jvs_http_request_t *request);

// #define jvs_str3_cmp(m, c0, c1, c2, c3)                                       \
//     *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)
// #define jvs_str3Ocmp(m, c0, c1, c2, c3)                                       \
//     *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)
// #define jvs_str4cmp(m, c0, c1, c2, c3)                                        \
//     *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#endif /* _JVS_HTTP_REQUEST_H_INCLUDED_ */