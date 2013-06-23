#ifndef ROOX_stack_H
#define ROOX_stack_H

#include <stdlib.h>
#include "stack_node.h"


typedef struct stack_struct {
    stack_node_t *head;
} stack_rt;


stack_rt *new_stack();
void stack_push (stack_rt *, void *);
void *stack_pop (stack_rt *);
void *stack_peak(stack_rt *);
int count(stack_rt *);
void empty_stack(stack_rt *);
void print_stack(stack_rt *);


#endif
