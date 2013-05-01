#ifndef ROOX_BBST_NODE_H
#define ROOX_BBST_NODE_H

typedef struct bbst_node_struct {
    int phrase_id;
    struct bbst_node_struct *left;
    struct bbst_node_struct *right;
} bbst_node_t;

typedef struct bbst_node_root_struct {
    bbst_node_t *root;
    int nodes_count;
} bbst_node_root_t;

bbst_node_t *new_bbst_node(int);

#endif

