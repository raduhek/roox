#ifndef ROOX_TRIE_NODE_H
#define ROOX_TRIE_NODE_H

#include "../bbst/bbst_node.h"
#include "../bbst/bbst.h"
#include "../list/list_node.h"

#define CHILDREN_BUCKETS 7
#define ROOT_ID 218
#define ROOT_CHAR '\0'

typedef struct trie_node_struct {
    char reaching_char;
    list_node_t *children[CHILDREN_BUCKETS];
    struct trie_node_struct *fail;
    struct trie_node_struct *parent;

    int is_root;

    bbst_node_root_t *AND;
    bbst_node_root_t *OR;
    bbst_node_root_t *NOT;
} trie_node_t;

trie_node_t *new_trie_node(char, trie_node_t *, unsigned short int);
trie_node_t *get_trie_node_children(const trie_node_t *, char);
unsigned short int is_trie_node_root(const trie_node_t *);
void add_info_in_trie_node(trie_node_t *, int, int, int);

#endif

