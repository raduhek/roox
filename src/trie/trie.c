#include <string.h>
#include <stdio.h>
#include <string.h>

#define WORD_CC word[word_idx]

#include "../bbst/bbst.h"
#include "../list/list.h"
#include "../queue/queue.h"
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
            temp = new_trie_node(WORD_CC, current, 0);
            list_add(&current->children[hash_char(WORD_CC)], WORD_CC, temp);
        }
        current = temp;
    }
    add_info_in_trie_node(current, 0, 0, 0);
}

void trie_compile(trie_node_t *root) {
    trie_node_t *current = root;
    trie_node_t *parent_fail = NULL;
    list_node_t *kid = NULL;
    queue_t *nodes = new_queue();
    int kid_idx;

    root->fail = root;
    push(nodes, (void*)root);
    while ((current = (trie_node_t*)pop(nodes)) != NULL) {
        // Add all children nodes in queue
        for (kid_idx = CHILDREN_BUCKETS - 1; kid_idx >= 0; --kid_idx) {
            kid = current->children[kid_idx];
            while (kid != NULL) {
                push(nodes, (void*)kid->to);
                kid = kid->next;
            }
        }

        if (current == root) {
            continue;
        }

        parent_fail = current->parent->fail;
        while (get_trie_node_children(parent_fail, current->reaching_char) != NULL &&
               parent_fail->fail != root ) {

            parent_fail = parent_fail->fail;
        }

        current->fail = get_trie_node_children(parent_fail, current->reaching_char);

        if (current->fail == NULL) {
            current->fail = get_trie_node_children(root, current->reaching_char);
        }
        if (current->fail == NULL || current->fail == current) {
            current->fail = root;
        }

    }

    return;
}

void trie_match(const trie_node_t *root, char *phrase, void (*callback)(const trie_node_t *)) {
    const trie_node_t *current = root;
    const trie_node_t *temp;
    while (*phrase) {

        while((temp = get_trie_node_children(current, *phrase)) == NULL && 
                current != root) {
            current = current->fail;
        }

        if (1 == is_trie_node_root(current)) {
            current = get_trie_node_children(root, *phrase);

            if (current == NULL) {
                current = root;
            }

            temp = current;
        } else {
            current = temp;
        }
    
        while (temp != root) {
            if (1 == is_trie_node_final(temp)) {
                callback(temp);
            }
            temp = temp->fail;
        }
        
        phrase++;
    }
}

