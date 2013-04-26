#ifndef ROOX_BBST_NODE_H
#define ROOX_BBST_NODE_H

typedef struct bbst_node_struct {
    int phrase_id;
    struct bbst_node_struct *left;
    struct bbst_node_struct *right;
} bbst_node_t;

bbst_node_t *new_bbst_node(int);

bbst_node_t *bbst_root;

#endif

