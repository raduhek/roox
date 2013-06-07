#include <stdlib.h>
#include <stdio.h>

#include "child_list.h"
#include "child_list_node.h"

void child_list_add(child_list_node_t **r, char c, trie_node_t *to) {
    child_list_node_t **t = r;
    child_list_node_t **parent = NULL;

    while (*t) {
        parent = t;
        if ((*t)->reach_char == c) {
            break;
        }
        t = &(*t)->next;
    }

    // If t is not NULL, it means this char has been added to the list
    if (*t != NULL) {
        return;
    }
    
    *t = new_child_list_node(c, to);

    // If parent is NULL, this is the first node in list
    if (parent == NULL) {
        return;
    }

    (*parent)->next = *t;
}

