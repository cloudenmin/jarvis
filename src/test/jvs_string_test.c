#include <jvs_core.h>

int jvs_strcasecmp_test(jvs_str_t *s1, jvs_str_t *s2)
{
    return jvs_strcasecmp(s1->data, s2->data);
}

int jvs_strncasecmp_test(jvs_str_t *s1, jvs_str_t *s2, size_t n)
{
    return jvs_strncasecmp(s1->data, s2->data, s1->size);
}

int jvs_str_trim_test(jvs_str_t *str)
{
    return jvs_str_trim(str);
}

jvs_str_t **jvs_str_split_test(jvs_str_t *str, u_char *separator)
{
    return jvs_str_split(str, separator);
}


int main() {
    jvs_str_t *str1 = (jvs_str_t *) malloc(sizeof(jvs_str_t));
    str1->data = "   str with blank    \n";
    str1->size = strlen("   str with blank    \n");
    printf("%s\n", str1->data);
    jvs_str_trim_test(str1);
    printf("%s\n", str1->data);
    u_char *separator = (u_char *)malloc(sizeof(u_char));
    strcpy(separator, " ");
    jvs_str_t **str_list = jvs_str_split_test(str1, separator);
    for(int i=0; i<3; i++) {
        printf("%s\n", ((jvs_str_t *)*(str_list+i))->data);
    }
    return 0;
}