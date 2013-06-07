#include <stdlib.h>

#include "list_node.h"

list_node_t *new_list_node(void *val) {
    list_node_t *t = (list_node_t*)malloc(sizeof(list_node_t));
    t->value = val;
    t->next = NULL;

    return t;
}

