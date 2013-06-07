#include <stdlib.h>

#include "child_list_node.h"

/*
    Initialize a new node for the binary tree
*/
child_list_node_t *new_child_list_node(char c, struct trie_node_struct *to) {
    child_list_node_t *t = (child_list_node_t*)malloc(sizeof(child_list_node_t));
    t->reach_char = c;
    t->next = NULL;
    t->to = to;

    return t;
}

