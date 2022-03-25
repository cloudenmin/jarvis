#include <jvs_core.h>

int main()
{
    u_char *data = "GET /api/test/123 http/1.1\r\nAccetp: application/json\r\nencode: utf-8\r\n";
    jvs_buffer_t *buff = (jvs_buffer_t *)malloc(sizeof(jvs_buffer_t));
    jvs_buffer_init(buff, data);
    u_char *line = jvs_buffer_read_line(buff);
    printf("%s\n", line);
    line = jvs_buffer_read_line(buff);
    printf("%s\n", line);
    line = jvs_buffer_read_line(buff);
    printf("%s\n", line);
    // printf("%s\n", buff->data);
    free(line);
    free(buff);
    return 0;
}