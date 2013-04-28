#include <stdlib.h>
#include <stdio.h>

#include "bbst.h"
#include "bbst_node.h"

void bbst_add(bbst_node_root_t **r, int val) {
    bbst_node_t **t = &(*r)->root;
    bbst_node_t **parent = NULL;

    // Traverse the tree until a NULL node appears
    // or the value to be inserted is found
    while(*t && (*t)->phrase_id != val) {
        parent = t;
        if (val < (*t)->phrase_id) {
            t = &(*t)->left;
        } else {
            t = &(*t)->right;
        }
    }

    if (*t == NULL) {
        *t = new_bbst_node(val);
        (*r)->nodes_count++;
    } else {
        return;
    }

    if (parent == NULL) {
        return;
    }
    if (val < (*parent)->phrase_id) {
        (*parent)->left = *t;
    } else {
        (*parent)->right = *t;
    }

}

