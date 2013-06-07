#ifndef ROOX_TRIE_NODE_H
#define ROOX_TRIE_NODE_H

#include "../child_list/child_list_node.h"

#define CHILDREN_BUCKETS 9
#define ROOT_ID 218
#define FINAL_NODE_ID 113
#define NORMAL_NODE_ID 17
#define ROOT_CHAR '\0'

typedef struct trie_node_struct {
    char reaching_char;
    child_list_node_t *children[CHILDREN_BUCKETS];
    struct trie_node_struct *fail;
    struct trie_node_struct *parent;

    int node_state;
} trie_node_t;

trie_node_t *new_trie_node(char, trie_node_t *, unsigned short int);
trie_node_t *get_trie_node_children(const trie_node_t *, char);
unsigned short int is_trie_node_root(const trie_node_t *);
unsigned short int is_trie_node_final(const trie_node_t *);

#endif

