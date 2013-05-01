#ifndef ROOX_QUEUE_NODE_H
#define ROOX_QUEUE_NODE_H

typedef struct queue_node_struct {
    const void *val;
    struct queue_node_struct *prev;
} queue_node_t;

queue_node_t *new_queue_node(const void *);

#endif
