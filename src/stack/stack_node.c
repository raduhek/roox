#include <stdlib.h>

#include "stack_node.h"

stack_node_t *new_stack_node(void *val, stack_node_t *prev) {
    stack_node_t *t = (stack_node_t*)malloc(sizeof(stack_node_t));
    t->val = val;
    t->prev = prev;

    return t;
}
