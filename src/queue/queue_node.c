#include <stdlib.h>

#include "queue_node.h"

queue_node_t *new_queue_node(void *val) {
    queue_node_t *t = (queue_node_t*)malloc(sizeof(queue_node_t));
    t->val = val;
    t->prev = NULL;

    return t;
}
