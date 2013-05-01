#include <stdlib.h>

#include "list_node.h"

/*
    Initialize a new node for the binary tree
*/
list_node_t *new_list_node(char c, struct trie_node_struct *to) {
    list_node_t *t = (list_node_t*)malloc(sizeof(list_node_t));
    t->reach_char = c;
    t->next = NULL;
    t->to = to;

    return t;
}

