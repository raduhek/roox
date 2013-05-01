#ifndef ROOX_QUEUE_H
#define ROOX_QUEUE_H

#include <stdlib.h>
#include "queue_node.h"


typedef struct queue_struct {
    queue_node_t *head;
    queue_node_t *tail;

} queue_t;

queue_t *new_queue() {
    queue_t *t = (queue_t*)malloc(sizeof(queue_t));
    t->head = NULL;
    t->tail = NULL;

    return t;
}

#endif
