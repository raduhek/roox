#include <stdio.h>
#include <stdlib.h>

#include "stack_node.h"
#include "stack.h"

void stack_push(stack_t *q, void *val) {
    stack_node_t *temp;
    if (q->head == NULL) {
        q->head = new_stack_node(val, NULL);
    } else {
        temp = new_stack_node(val, q->head);
        q->head = temp;
    }
}

void *stack_pop(stack_t *q) {
    stack_node_t *temp;
    void *val;
    if (q->head == NULL) {
        return NULL;
    } else {
        temp = q->head;
        val = temp->val;
        q->head = q->head->prev;
        free(temp);
        return val;
    }

}

int count(stack_t *q) {
    stack_node_t *temp = q->head;
    int i = 0;
    while (temp) {
        i++;
        temp = temp->prev;
    }
    return i;
}

void empty_stack(stack_t *q) {
    stack_node_t *t = q->head;
    while (t) {
        q->head = t->prev;
        free(t);
        t = q->head;
    }
}

void *stack_peak(stack_t *q) {
    if (q->head == NULL) {
        return NULL;
    }
    return q->head->val;
}

stack_t *new_stack() {
    stack_t *t = (stack_t*)malloc(sizeof(stack_t));
    t->head = NULL;
    return t;
}

