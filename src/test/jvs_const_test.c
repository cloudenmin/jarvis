#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

int main() {
    int a;
    a = 2;
    printf("-----%d\n", a);
    char *tmp = (char *)malloc(sizeof(char *) * 10);
    memcpy(tmp, "1234567", 7);
    const char *address = malloc(sizeof(char *) * 10);;
    memcpy(address, "qwertyi", 7);
    printf("%s-%p\n", address, address);
    memcpy(address, "mmmmmmm", 7);
    printf("%s-%p\n", address, address);
    address = tmp;
    //memcpy(address, "qttttyi", 7);
    printf("%s-%p\n", address, address);
}