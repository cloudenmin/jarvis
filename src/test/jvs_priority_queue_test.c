#include <jvs_priority_queue.h>
#include <malloc.h>

typedef struct jvs_test_item {
    int key;
}jvs_test_item_t;


int test_comp(void* ti, void* tj){
    jvs_test_item_t* i = (jvs_test_item_t*)ti;
    jvs_test_item_t* j = (jvs_test_item_t*)tj;
    return (i->key < j->key) ? 1 : 0;
}

void print_queue(jvs_pq_t *jvs_pq) {
    for (int i = 0; i < jvs_pq->size; i++) {
        jvs_test_item_t *item = (jvs_test_item_t *) jvs_pq->pq[i];
        printf("%p-%d-%d\n", item, i, item->key);
    }
}

int main() {
    int array[15] = {100, 99, 95, 90, 85, 80, 75, 70, 65, 44, 9, 7, 2, 1, 0};
    jvs_pq_t *jvs_pq = (jvs_pq_t *) malloc(sizeof(jvs_pq_t *));
    jvs_pq_init(jvs_pq, test_comp, 50);
    for (int i = 0; i < 15; i++) {
        jvs_test_item_t *item = (jvs_test_item_t *)malloc(sizeof(jvs_test_item_t *));
        item->key = array[i];
        jvs_pq_insert(jvs_pq, (void *)item);
    }
    printf("=============print priotity queue==============\n");
    print_queue(jvs_pq);

    printf("==============print sorted queue===============\n");
    size_t q_size = jvs_pq->size;
    for (size_t i = 0; i < q_size; i++) {
        jvs_test_item_t *item_min = (jvs_test_item_t *)jvs_pq_min(jvs_pq);
        printf("%p-%d\n", item_min, item_min->key);
        jvs_pq_delmin(jvs_pq);
    }
    return 1;
}
 