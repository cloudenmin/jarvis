#ifndef _JVS_BUFFER_IO_H_INCLUDED_
#define _JVS_BUFFER_IO_H_INCLUDED_

#include <jvs_core.h>

struct jvs_buffer
{
    u_char *data;
    int  r;
    int  w;
};

u_char *jvs_buffer_read_line(jvs_buffer_t *buf);

#define jvs_buffer_init(buff, src)        \
        (buff)->data = src;               \
        (buff)->r = 0;                     \
        (buff)->w = strlen(src) + 1;
#endif /* _JVS_BUFFER_IO_H_INCLUDED_ */