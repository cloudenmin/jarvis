#include <jvs_core.h>

u_char *jvs_buffer_read_line(jvs_buffer_t *buf)
{
    if(buf->r == buf->w) {
        return NULL;
    }
    u_char *start = buf->data + buf->r;
    u_char *pos = (u_char *)strchr(start, '\n');
    buf->r = pos - buf->data + 1;
    int read_num = pos - start;
    if(*(pos-1) == '\r') {
        --read_num;
    }
    u_char *line = (u_char *)malloc(sizeof(u_char) * (read_num + 1));
    memset(line, 0, read_num + 1);
    memcpy(line, start, read_num);
    return line;
}

