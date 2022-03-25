#include <jvs_core.h>


int main() {
    jvs_hash_t map;
    jvs_hash_init(&map, JVS_DEFAULT_CAPACITY);
    jvs_str_t key = {"test", 4};
    jvs_str_t value = {"qwer", 4};
    jvs_hash_set(&map, key, &value);
    jvs_str_t *result = (jvs_str_t *)jvs_hash_get(&map, key);
    printf("%s\n", result->data);
    free(map.table);
    return JVS_OK;
}
