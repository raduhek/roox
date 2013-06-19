#ifndef ROOX_TRIE_H
#define ROOX_TRIE_H

#include "trie_node.h"

typedef struct parse_tree_struct pts;

void trie_insert(trie_node_t *, const char *, void *);
void trie_compile(trie_node_t *);
void trie_match(const trie_node_t *, const char *, short int, void(*)(struct parse_tree_struct *, short int));

#endif

