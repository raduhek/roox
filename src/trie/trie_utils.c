#include "trie_utils.h"
#include "trie_node.h"

inline int abs(int i) {
    return i >= 0 ? i : -i;
}

inline int hash_char(char c) {
    return abs((int)c % CHILDREN_BUCKETS);
}
