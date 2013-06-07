#ifndef ROOX_LIST_NODE_H
#define ROOX_LIST_NODE_H

typedef struct node_struct {
    void *value;
    struct list_node_struct *next;
} list_node_t;

list_node_t *new_list_node(void *);

#endif

