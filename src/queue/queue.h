#ifndef ROOX_QUEUE_H
#define ROOX_QUEUE_H

#include <stdlib.h>
#include "queue_node.h"


typedef struct queue_struct {
    queue_node_t *head;
    queue_node_t *tail;

} queue_t;


queue_t *new_queue();
void push (queue_t *, const void *);
void *pop (queue_t *);

#endif
