#include <jvs_core.h>

static int switch_method(jvs_str_t *method)
{
    if(jvs_strcasecmp(method->data, "GET") == 0) {
        return JVS_HTTP_GET;
    } else if (jvs_strcasecmp(method->data, "POST") == 0) {
        return JVS_HTTP_POST;
    }
    return JVS_HTTP_UNKNOWN;
}

static int jvs_http_parse_request_line(jvs_buffer_t *buffer, jvs_http_request_t *request) 
{
    u_char *line = jvs_buffer_read_line(buffer);
    if (line == NULL) {
        return JVS_ERROR;
    }
    jvs_str_t line_str = jvs_string(line);
    char delim[] = " ";
    jvs_str_t **tokens = jvs_str_split(&line_str, delim);
    if (tokens == NULL) {
        return JVS_ERROR;
    }
    size_t tokens_len = sizeof(*tokens);
    jvs_str_t *method = (jvs_str_t *)*(tokens+0);
    request->method = switch_method(method);
    request->uri = *tokens[1];
    request->protocol = **(tokens+2);
    free(tokens);
    return JVS_OK;
}

static int jvs_http_parse_request_head(jvs_buffer_t *buffer, jvs_http_request_t *request)
{
    jvs_hash_t *header = (jvs_hash_t *)malloc(sizeof(jvs_hash_t));
    jvs_hash_init(header, 10);
    request->header = header;
    char delim[] = ":";
    u_char *line = jvs_buffer_read_line(buffer);
    jvs_str_t line_str = {line, strlen(line)};
    while (strcasecmp(line, "\r\n") > 0) {
        jvs_str_t **tokens = jvs_str_split(&line_str, delim);
        jvs_str_t *key = (jvs_str_t *)*(tokens+0);
        jvs_str_trim(key);
        jvs_str_t *value = (jvs_str_t *)*(tokens+1);
        jvs_str_trim(value);
        jvs_hash_set(request->header, *key, value);
        line = jvs_buffer_read_line(buffer);
        jvs_str_t line_str = {line, strlen(line)};
    }
    return JVS_OK;
}

static int jvs_http_get_body_from_buffer(jvs_buffer_t *buffer, jvs_http_request_t *request)
{
    int body_len = buffer->w - buffer->r + 1;
    if (request->body == NULL) {
        request->body = (u_char *)malloc(sizeof(u_char) * body_len);
    }
    memcpy(request->body, buffer->data + buffer->r, body_len);
}

int jvs_http_parse_request(jvs_http_connect_t *conn, jvs_http_request_t *request)
{
    jvs_buffer_t buffer;
    jvs_buffer_init(&buffer, conn->read_buff);
    jvs_http_parse_request_line(&buffer, request);
    jvs_http_parse_request_head(&buffer, request);
    jvs_http_get_body_from_buffer(&buffer, request);
    return JVS_OK;
}