#include <jvs_core.h>

void jvs_hash_init(jvs_hash_t *hash, int capacity) {
    hash->table = (jvs_entry_t **)malloc(sizeof(jvs_entry_t *) * capacity);
    for(int i = 0; i < capacity; i++) {
        *(hash->table + 1) = NULL;
    }
    hash->size = 0;  
    hash->capacity = capacity;  
    hash->load_factor = JVS_DEFAULT_LOAD_FACTOR;  
    hash->threshold = capacity * JVS_DEFAULT_LOAD_FACTOR;
}

static int jvs_hash_key(u_char *data, size_t len) {
    int  i, key;
    key = 0;
    for (i = 0; i < len; i++) {
        key = jvs_hash(key, data[i]);
    }
    return key;
}

static void transfer(jvs_hash_t *hash, jvs_entry_t **new_table, int new_capacity) {
    for(int i = 0; i <= hash->capacity - 1; i++) {
        for(jvs_entry_t *entry = *(hash->table + i); entry != NULL;) {
            entry->hash_code = jvs_hash_key(entry->key.data, entry->key.size);
            int index = entry->hash_code & (new_capacity - 1);
            jvs_entry_t *next = entry->next;
            jvs_entry_t **bucket = new_table + index;
            entry->next = *bucket;
            *bucket = entry;
            entry = next;
        }
    }
}

static void resize(jvs_hash_t *hash, int new_capacity) {
    jvs_entry_t **old_table = hash->table;
    int old_capacity = hash->capacity;
    if (old_capacity == JVS_MAXNUM_CAPACITY) {
        hash->threshold = JVS_MAXNUM_CAPACITY + 1;
        return;
    }
    jvs_entry_t **new_table = (jvs_entry_t **)malloc(sizeof(jvs_entry_t*) * new_capacity);
    transfer(hash, new_table, new_capacity);
    hash->table = new_table;
    hash->capacity = new_capacity;
    int new_threshold = new_capacity * hash->load_factor;
    int com_capacity = JVS_MAXNUM_CAPACITY + 1;
    hash->threshold = new_threshold < com_capacity ? new_threshold : com_capacity;
}

static void jvs_add_entry(jvs_hash_t *hash, jvs_str_t key, void *value, int hash_code, int index) {
    if(hash->size >= hash->threshold && *(hash->table + index) != NULL) {
        resize(hash, hash->capacity * 2);
    }
    jvs_entry_t *add_entry = (jvs_entry_t *)malloc(sizeof(jvs_entry_t));
    add_entry->hash_code = hash_code;
    add_entry->key = key;
    add_entry->value = value;
    jvs_entry_t **bucket = hash->table + index;
    add_entry->next = *bucket;
    *bucket = add_entry;
    hash->size++;
}

int jvs_hash_set(jvs_hash_t *hash, jvs_str_t key, void *value) {
    if (key.data == NULL) {
        return JVS_ERROR;
    }

    int hash_code = jvs_hash_key(key.data, key.size);

    int index = hash_code & (hash->capacity - 1);

    for(jvs_entry_t *entry = *(hash->table + index); entry != NULL; entry=entry->next) {
        if(jvs_strcasecmp(key.data, entry->key.data) == 0) {
            entry->value = value;
            return JVS_OK;
        }
    }
    jvs_add_entry(hash, key, value, hash_code, index);
    return JVS_OK;
}

void *jvs_hash_get(jvs_hash_t *hash, jvs_str_t key) {
    if(key.data == NULL) {
        return NULL;
    }
    int hash_code = jvs_hash_key(key.data, key.size);

    int index = hash_code & (hash->capacity - 1);

    for(jvs_entry_t *entry = *(hash->table + index); entry != NULL; entry=entry->next) {
        if(entry->hash_code == hash_code && 
           jvs_strcasecmp(entry->key.data, key.data) == 0 && 
           entry->key.size == key.size) {
            return entry->value;
        }
    }
    return NULL;
}