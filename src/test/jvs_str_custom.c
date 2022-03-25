#include <string.h>
#include <stdio.h>
#include <malloc.h>

typedef unsigned char   u_char;
typedef u_char *          string;

int main()
{
    string str1 = "qwertyuu";
    string str2 = (string) malloc(sizeof(u_char)*8);
    string str3 = (string) malloc(sizeof(u_char)*8);
    strcpy(str2, str1);
    strcpy(str3, "asdf");
    u_char str4[] = "poiuyt";
    str1 = str4;
    printf("str1--%s\n", str1);
    printf("str2--%s\n", str2);
    // *(str2+7) = ' ';
    *(str2+8) = ' ';
    *(str2+34) = 'c';
    printf("str2--%s\n", str2);
    printf("str3--%s\n", str3);
    printf("str3 len %ld\n", strlen(str3));
    printf("str3 len %ld\n", sizeof(str3));
    printf("str3 len %ld\n", sizeof("1234"));
    return 0;
}