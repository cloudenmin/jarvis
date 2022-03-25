#include <jvs_core.h>

// Create a new node
jvs_rbtree_node_t *jvs_new_node(void *val)
{
    jvs_rbtree_node_t *create = (jvs_rbtree_node_t *)(malloc(sizeof(jvs_rbtree_node_t *)));
    create->val = val;
    create->parent = NULL;
    create->left = NULL;
    create->right = NULL;
    jvs_rbt_red(create);
    return create;
}


/* 
 *  left rotate for x
 *      px                                 px
 *     /                                  /
 *    x                                  y                
 *   /  \      --(left rotate)-->       / \                
 *  lx   y                             x  ry     
 *     /   \                         /  \
 *    ly   ry                       lx  ly  
 *
 *
 */
static void jvs_left_rotate(jvs_rbtree_node_t **root,
                            jvs_rbtree_node_t *sentinel,
                            jvs_rbtree_node_t *node)
{
    jvs_rbtree_node_t  *temp;

    temp = node->right;
    node->right = temp->left;

    if (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;

    } else if (node == node->parent->left) {
        node->parent->left = temp;

    } else {
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}

/* 
 *
 *  right rotate for y
 *
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \      --(right rotate)-->    /  \                     
 *        x   ry                           lx   y  
 *       / \                                   / \                   
 *      lx  rx                                rx  ry
 * 
 */
static void jvs_rbtree_right_rotate(jvs_rbtree_node_t **root,
                                    jvs_rbtree_node_t *sentinel,
                                    jvs_rbtree_node_t *node)
{
    jvs_rbtree_node_t  *temp;

    temp = node->left;
    node->left = temp->right;

    if (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;

    } else if (node == node->parent->right) {
        node->parent->right = temp;

    } else {
        node->parent->left = temp;
    }

    temp->right = node;
    node->parent = temp;
}

static void jvs_rbtree_insert_value(jvs_rbtree_node_t *temp, 
                                    jvs_rbtree_node_t *node,
                                    jvs_rbtree_node_t *sentinel)
{
    jvs_rbtree_node_t  **p;

    for ( ;; ) {

        p = (node->key < temp->key) ? &temp->left : &temp->right;

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    jvs_rbt_red(node);
}

// To insert a node in the existing tree
void jvs_rbtree_insert(jvs_rbtree_t *tree, jvs_rbtree_node_t *node) {
    jvs_rbtree_node_t  **root, *temp, *sentinel;

    /* a binary tree insert */

    root = &tree->root;
    sentinel = tree->sentinel;

    if (*root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        jvs_rbt_black(node);
        *root = node;

        return;
    }

    jvs_rbtree_insert_value(*root, node, sentinel);

    /* re-balance tree */

    while (node != *root && jvs_rbt_is_red(node->parent)) {

        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            if (jvs_rbt_is_red(temp)) {
                jvs_rbt_black(node->parent);
                jvs_rbt_black(temp);
                jvs_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    jvs_rbtree_left_rotate(root, sentinel, node);
                }

                jvs_rbt_black(node->parent);
                jvs_rbt_red(node->parent->parent);
                jvs_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } else {
            temp = node->parent->parent->left;

            if (jvs_rbt_is_red(temp)) {
                jvs_rbt_black(node->parent);
                jvs_rbt_black(temp);
                jvs_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    jvs_rbtree_right_rotate(root, sentinel, node);
                }

                jvs_rbt_black(node->parent);
                jvs_rbt_red(node->parent->parent);
                jvs_rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }

    jvs_rbt_black(*root);
}

// To delete a node from the tree
void jvs_rbtree_delete(jvs_rbtree_t *tree, jvs_rbtree_node_t *node) {
    int           red;
    jvs_rbtree_node_t  **root, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    root = &tree->root;
    sentinel = tree->sentinel;

    if (node->left == sentinel) {
        temp = node->right;
        subst = node;

    } else if (node->right == sentinel) {
        temp = node->left;
        subst = node;

    } else {
        subst = jvs_rbtree_min(node->right, sentinel);
        temp = subst->right;
    }

    if (subst == *root) {
        *root = temp;
        jvs_rbt_black(temp);

        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

        return;
    }

    red = jvs_rbt_is_red(subst);

    if (subst == subst->parent->left) {
        subst->parent->left = temp;

    } else {
        subst->parent->right = temp;
    }

    if (subst == node) {

        temp->parent = subst->parent;

    } else {

        if (subst->parent == node) {
            temp->parent = subst;

        } else {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        jvs_rbt_copy_color(subst, node);

        if (node == *root) {
            *root = subst;

        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }

        if (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        if (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    if (red) {
        return;
    }

    /* a delete fixup */

    while (temp != *root && jvs_rbt_is_black(temp)) {

        if (temp == temp->parent->left) {
            w = temp->parent->right;

            if (jvs_rbt_is_red(w)) {
                jvs_rbt_black(w);
                jvs_rbt_red(temp->parent);
                jvs_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            if (jvs_rbt_is_black(w->left) && jvs_rbt_is_black(w->right)) {
                jvs_rbt_red(w);
                temp = temp->parent;

            } else {
                if (jvs_rbt_is_black(w->right)) {
                    jvs_rbt_black(w->left);
                    jvs_rbt_red(w);
                    jvs_rbtree_right_rotate(root, sentinel, w);
                    w = temp->parent->right;
                }

                jvs_rbt_copy_color(w, temp->parent);
                jvs_rbt_black(temp->parent);
                jvs_rbt_black(w->right);
                jvs_rbtree_left_rotate(root, sentinel, temp->parent);
                temp = *root;
            }

        } else {
            w = temp->parent->left;

            if (jvs_rbt_is_red(w)) {
                jvs_rbt_black(w);
                jvs_rbt_red(temp->parent);
                jvs_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            if (jvs_rbt_is_black(w->left) && jvs_rbt_is_black(w->right)) {
                jvs_rbt_red(w);
                temp = temp->parent;

            } else {
                if (jvs_rbt_is_black(w->left)) {
                    jvs_rbt_black(w->right);
                    jvs_rbt_red(w);
                    jvs_rbtree_left_rotate(root, sentinel, w);
                    w = temp->parent->left;
                }

                jvs_rbt_copy_color(w, temp->parent);
                jvs_rbt_black(temp->parent);
                jvs_rbt_black(w->left);
                jvs_rbtree_right_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        }
    }

    jvs_rbt_black(temp);
}


void checkBlack(jvs_node_t *temp, int c)
{
    if (temp == NULL)
    {
        printf("%d ", c);
        return;
    }
    if (temp->color == 0)
    {
        c++;
    }
    checkBlack(temp->left, c);
    checkBlack(temp->right, c);
}

// int main()
// {
//     jvs_node_t *root = NULL;
//     int scanValue, choice = 1;
//     printf(
//         "1 - Input\n2 - Delete\n3 - Inorder Traversel\n0 - Quit\n\nPlease "
//         "Enter the Choice - ");
//     scanf("%d", &choice);
//     while (choice)
//     {
//         switch (choice)
//         {
//         case 1:
//             printf("\n\nPlease Enter A Value to insert - ");
//             scanf("%d", &scanValue);
//             if (root == NULL)
//             {
//                 root = newjvs_node_t(scanValue, NULL);
//                 root->color = 0;
//             }
//             else
//             {
//                 insertjvs_node_t(scanValue, &root);
//             }
//             printf("\nSuccessfully Inserted %d in the tree\n\n", scanValue);
//             break;
//         case 2:
//             printf("\n\nPlease Enter A Value to Delete - ");
//             scanf("%d", &scanValue);
//             deletejvs_node_t(scanValue, &root);
//             printf("\nSuccessfully Inserted %d in the tree\n\n", scanValue);
//             break;
//         case 3:
//             printf("\nInorder Traversel - ");
//             printInorder(root);
//             printf("\n\n");
//             // checkBlack(root,0);
//             // printf("\n");
//             break;
//         default:
//             if (root != NULL)
//             {
//                 printf("Root - %d\n", root->val);
//             }
//         }
//         printf(
//             "1 - Input\n2 - Delete\n3 - Inorder Traversel\n0 - "
//             "Quit\n\nPlease Enter the Choice - ");
//         scanf("%d", &choice);
//     }
// }