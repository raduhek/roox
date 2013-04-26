#include <stdlib.h>

#include "bbst_node.h"

/*
    Initialize a new node for the binary tree
*/
bbst_node_t *new_bbst_node(int pid) {
    bbst_node_t *t = (bbst_node_t*)malloc(sizeof(bbst_node_t));
    t->phrase_id = pid;
    t->left = NULL;
    t->right = NULL;

    return t;
}

