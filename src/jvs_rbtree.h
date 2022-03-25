#ifndef _JVS_RBTREE_INCLUDED_
#define _JVS_RBTREE_INCLUDED_

#include <jvs_core.h>

#define RED   1
#define BLACK 0

typedef struct jvs_rbtree_node {
    void            *val;
    struct jvs_node *parent;
    struct jvs_node *left;
    struct jvs_node *right;
    int color;
} jvs_rbtree_node_t;

typedef struct jvs_rbtree{
    jvs_rbtree_node_t *root;
    jvs_rbtree_node_t *sentinel;
}jvs_rbtree_t;

jvs_rbtree_node_t *jvs_new_node(void *val);
void jvs_rbtree_insert(jvs_rbtree_t *tree, jvs_rbtree_node_t *node);
void jvs_rbtree_delete(jvs_rbtree_t *tree, jvs_rbtree_node_t *node);


#define jvs_rbt_red(node)               ((node)->color = RED)
#define jvs_rbt_black(node)             ((node)->color = BLACK)
#define jvs_rbt_is_red(node)            ((node)->color)
#define jvs_rbt_is_black(node)          (!jvs_rbt_is_red(node))

#define jvs_rbtree_sentinel_init(node)  jvs_rbt_black(node)

#define jvs_rbtree_init(tree, s)    \
    jvs_rbtree_sentinel_init(s);    \
    (tree)->root = s;               \
    (tree)->sentinel = s;

#endif /* _JVS_RBTREE_INCLUDED_ */