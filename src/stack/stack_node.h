#ifndef ROOX_stack_NODE_H
#define ROOX_stack_NODE_H

typedef struct stack_node_struct {
    void *val;
    struct stack_node_struct *prev;
} stack_node_t;

stack_node_t *new_stack_node(void *, stack_node_t *);

#endif
