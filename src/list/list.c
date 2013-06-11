#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "list_node.h"

void list_add(list_node_t **r, void *val) {
    list_node_t *entry = new_list_node(val);
    entry->next = *r;
    *r = entry;
}

