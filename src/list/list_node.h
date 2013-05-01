#ifndef ROOX_LIST_NODE_H
#define ROOX_LIST_NODE_H

struct trie_node_struct;

typedef struct list_node_struct {
    char reach_char;
    struct list_node_struct *next;
    struct trie_node_struct *to;
} list_node_t;

list_node_t *new_list_node(char, struct trie_node_struct *);

#endif

