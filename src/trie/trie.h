#ifndef ROOX_TRIE_H
#define ROOX_TRIE_H

#include "trie_node.h"

void trie_insert(trie_node_t *, const char *);
void trie_compile(trie_node_t **);
void trie_search(trie_node_t **, const char *);

#endif

