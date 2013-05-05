#include <stdlib.h>
#include <stdio.h>

#include "trie_node.h"
#include "trie_utils.h"
#include "../bbst/bbst.h"
#include "../list/list_node.h"

trie_node_t *new_trie_node(char reaching_char, trie_node_t *parent, unsigned short int state) {
    int i = 0;
    trie_node_t *t = (trie_node_t*)malloc(sizeof(trie_node_t));

    for (i = CHILDREN_BUCKETS - 1; i >= 0; --i) {
        t->children[i] = NULL;
    }
    t->AND = t->OR = t->NOT = (bbst_node_root_t*)malloc(sizeof(bbst_node_root_t));
    t->node_state = state;
    t->parent = parent;
    t->reaching_char = reaching_char;

    return t;
}

trie_node_t *get_trie_node_children(const trie_node_t *t, char c) {
    unsigned short int kid_bucket = hash_char(c);
 
    list_node_t *ret = t->children[kid_bucket];

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

void add_info_in_trie_node(trie_node_t *node, int phrase_id_AND, int phrase_id_OR, int phrase_id_NOT) {
    node->node_state = FINAL_NODE_ID;

    bbst_add(&node->AND, phrase_id_AND);
    bbst_add(&node->OR, phrase_id_OR);
    bbst_add(&node->NOT, phrase_id_NOT);
}
