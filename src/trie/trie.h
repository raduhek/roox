#ifndef ROOX_TRIE_H
#define ROOX_TRIE_H

#include "trie_node.h"

struct parse_tree_struct;
void validate_tree(struct parse_tree_struct *, void (*c)());

void trie_insert(trie_node_t *, const char *, void *);
void trie_compile(trie_node_t *);
void trie_match(const trie_node_t *, const char *, void (*c)());

#endif

