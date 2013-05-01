#include <stdio.h>
#include <stdlib.h>

#include "queue_node.h"
#include "queue.h"

void push(queue_t *q, const void *val) {
    queue_node_t *temp;
    if (q->head == NULL) {
        q->head = new_queue_node(val);
        q->tail = q->head;
    } else {
        temp = new_queue_node(val);
        q->tail->prev = temp;
        q->tail = temp;
    }
}

void *pop(queue_t *q) {
    queue_node_t *temp;
    if (q->head == NULL) {
        return NULL;
    } else {
        temp = q->head;
        q->head = q->head->prev;
        return temp;
    }

}

