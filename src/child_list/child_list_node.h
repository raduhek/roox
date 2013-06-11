#ifndef ROOX_CHILD_LIST_NODE_H
#define ROOX_CHILD_LIST_NODE_H

struct trie_node_struct;

typedef struct child_list_node_struct {
    char reach_char;
    struct child_list_node_struct *next;
    struct trie_node_struct *to;
} child_list_node_t;

child_list_node_t *new_child_list_node(char, struct trie_node_struct *);

#endif

