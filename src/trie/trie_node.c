#include <stdlib.h>
#include <stdio.h>

#include "trie_node.h"
#include "trie_utils.h"
#include "../child_list/child_list_node.h"

trie_node_t *new_trie_node(char reaching_char, trie_node_t *parent, unsigned short int state) {
    int i = 0;
    trie_node_t *t = (trie_node_t*)malloc(sizeof(trie_node_t));

    for (i = CHILDREN_BUCKETS - 1; i >= 0; --i) {
        t->children[i] = NULL;
    }
    t->node_state = state;
    t->parent = parent;
    t->reaching_char = reaching_char;

    t->syntax_trees = NULL;
    return t;
}

trie_node_t *get_trie_node_children(const trie_node_t *t, char c) {
    unsigned short int kid_bucket = hash_char(c);
 
    child_list_node_t *ret = t->children[kid_bucket];

    while (ret && ret->reach_char != c) {
        ret = ret->next;
    }

    if (ret) {
        return ret->to;
    }

    return NULL;
}

inline unsigned short int is_trie_node_root(const trie_node_t *t) {
    return ROOT_ID == t->node_state && NULL == t->parent;
}

inline unsigned short int is_trie_node_final(const trie_node_t *t) {
    return FINAL_NODE_ID == t->node_state;
}

