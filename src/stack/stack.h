#ifndef ROOX_stack_H
#define ROOX_stack_H

#include <stdlib.h>
#include "stack_node.h"


typedef struct stack_struct {
    stack_node_t *head;
} stack_t;


stack_t *new_stack();
void stack_push (stack_t *, void *);
void *stack_pop (stack_t *);
void *stack_peak(stack_t *);
int count(stack_t *);
void empty_stack(stack_t *);
void print_stack(stack_t *);


#endif
