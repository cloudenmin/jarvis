#ifndef _JVS_STRING_INCLUDED_
#define _JVS_STRING_INCLUDED_

#include <jvs_core.h>

struct jvs_string {
    u_char *data;
    size_t  size;
};


#define jvs_string(str)     { str, sizeof(str) - 1 }
#define jvs_str_set(str, text)                                               \
    (str)->data = (u_char *) text; (str)->len = sizeof(text) - 1;
#define jvs_str_null(str)   (str)->data = NULL; (str)->len = 0;

#define jvs_strncmp(s1, s2, n)  strncmp((const char *) s1, (const char *) s2, n)
#define jvs_strcmp(s1, s2)  strcmp((const char *) s1, (const char *) s2)

#define jvs_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define jvs_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

int jvs_strcasecmp(u_char *s1, u_char *s2);
int jvs_strncasecmp(u_char *s1, u_char *s2, size_t n);
int jvs_str_trim(jvs_str_t *str);
jvs_str_t **jvs_str_split(jvs_str_t *str, u_char *separator);
int jvs_str_destroy(jvs_str_t *str, int mode);
#endif /* _JVS_STRING_INCLUDED_ */