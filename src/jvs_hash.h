#ifndef _JVS_HASH_INCLUDED_
#define _JVS_HASH_INCLUDED_

#include <jvs_core.h>

#define JVS_DEFAULT_CAPACITY 16

#define JVS_DEFAULT_LOAD_FACTOR 0.75

#define JVS_MAXNUM_CAPACITY 1 << 30


struct jvs_entry {
    int          hash_code;
    jvs_str_t    key;
    void         *value;
    jvs_entry_t  *next;
};


struct jvs_hash {
    jvs_entry_t **table;
    int         size;
    int         capacity;
    float       load_factor;
    int         threshold;
};       

void jvs_hash_init(jvs_hash_t *hash, int capacity);
int jvs_hash_set(jvs_hash_t *hash, jvs_str_t key, void *value);
void *jvs_hash_get(jvs_hash_t *hash, jvs_str_t key);

#define jvs_hash(key, c)   (key * 31 + c)

#endif /* _JVS_HASH_INCLUDED_ */