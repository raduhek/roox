#include <string.h>
#include <stdio.h>

#define WORD_CC word[word_idx]

#include "../bbst/bbst.h"
#include "../list/list.h"
#include "trie_node.h"
#include "trie_utils.h"

void trie_insert(trie_node_t *root, char *word) {
    int word_idx = 0;
    int word_length = strlen(word);
    trie_node_t *current = root;
    trie_node_t *temp;

    for (word_idx = 0; word_idx < word_length; ++word_idx) {
        temp = get_trie_node_children(current, WORD_CC);
        if (temp == NULL) {
            temp = new_trie_node(0);
            list_add(&current->children[hash_char(WORD_CC)], WORD_CC, temp);
        }
        current = temp;
    }
}

void trie_search(const trie_node_t *root, const char *word) {
    int word_idx = 0;
    int word_length = strlen(word);
    const trie_node_t *current = root;

    for (word_idx = 0; word_idx < word_length; ++word_idx) {
        current = get_trie_node_children(current, WORD_CC);
        if (current == NULL) {
            return;
        }
    }
    printf("found word: %s\n", word);
    return;
}
