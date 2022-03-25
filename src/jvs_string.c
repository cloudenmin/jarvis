#include <jvs_core.h>

int jvs_strcasecmp(u_char *s1, u_char *s2)
{
    unsigned int  c1, c2;

    for ( ;; ) {
        c1 = (unsigned int) *s1++;
        c2 = (unsigned int) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        if (c1 == c2) {
            if (c1) {
                continue;
            }
            return 0;
        }
        return c1 - c2;
    }
}


int jvs_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    unsigned int  c1, c2;

    while (n) {
        c1 = (unsigned int) *s1++;
        c2 = (unsigned int) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        if (c1 == c2) {

            if (c1) {
                n--;
                continue;
            }
            return 0;
        }
        return c1 - c2;
    }
    return 0;
}


int jvs_str_trim(jvs_str_t *str)
{
    u_char *head = str->data;
    u_char *tail = str->data + str->size -1;
    while (*head == ' ' || *head == '\n' || *head == '\r')
    {
        ++head;
    }

    while (*tail == ' '|| *tail == '\n' || *tail == '\r')
    {
        --tail;
    }

    size_t new_size = tail - head + 1;
    u_char *new_str = (u_char *)malloc(sizeof(u_char) * new_size);
    strncpy(new_str, head, new_size);
    str->data = new_str;
    str->size = new_size;
    return JVS_OK;
}

jvs_str_t **jvs_str_split(jvs_str_t *str, u_char *separator)
{
    int init_cap = 4;
    jvs_str_t **result = malloc(sizeof(jvs_str_t *) * init_cap);
    memset(result, 0, 4);
    char *p = (char *)malloc(sizeof(char) * str->size);
    strcpy(p, str->data);

    u_char *token = (u_char *)strsep(&p, separator);
    if(token == NULL) {
        return NULL;
    }
    int count = 0;
    while (token) {
        if(count > init_cap) {
            init_cap *= 2;
            result == realloc(result, init_cap);
        }
        jvs_str_t *sub_str = (jvs_str_t *)malloc(sizeof(jvs_str_t));
        sub_str->data = token;
        sub_str->size = strlen(token);
        *(result + count) = sub_str;
        count++;
        token = strsep(&p, separator);
    }
    return result;
}
