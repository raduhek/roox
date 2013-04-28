#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "list_node.h"

void list_add(list_node_t **r, char c) {
    list_node_t **t = r;
    list_node_t **parent = NULL;

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
    
    *t = new_list_node(c);

    // If parent is NULL, this is the first node in list
    if (parent == NULL) {
        return;
    }

    (*parent)->next = *t;
}

