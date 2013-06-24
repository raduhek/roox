#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define WORD_CC word[word_idx]

#include "../child_list/child_list.h"
#include "../list/list.h"
#include "../queue/queue.h"
#include "trie.h"
#include "trie_node.h"
#include "trie_utils.h"

struct parse_tree_struct;

extern void validate_tree();
extern int operator_truth_value();
extern struct trie_node_struct *trie_root;
extern int text_match_index;
extern list_node_t *syntax_trees;

void trie_insert(trie_node_t *root, const char *word, void *data) {
    int word_idx = 0;
    int word_length = strlen(word);
    trie_node_t *current = root;
    trie_node_t *temp;

    for (word_idx = 0; word_idx < word_length; ++word_idx) {
        temp = get_trie_node_children(current, WORD_CC);
        if (temp == NULL) {
            temp = new_trie_node(WORD_CC, current, 0);
            child_list_add(&current->children[hash_char(WORD_CC)], WORD_CC, temp);
        }
        current = temp;
    }

    current->node_state = FINAL_NODE_ID;
    list_add(&current->syntax_trees, data);
}

void trie_compile(trie_node_t *root) {
    trie_node_t *current = root;
    trie_node_t *parent_fail = NULL;
    child_list_node_t *kid = NULL;
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

/*void trie_match(        char *uid,
                        const trie_node_t *root,
                        char *phrase, 
                        short int switcher,
                        void (*cb)()) {*/
//void trie_match(char *uid, ...) {
int trie_match(char *udid, char *phrase) {

    const trie_node_t *current = trie_root;
    const trie_node_t *temp;
    list_node_t *st;

    while (*phrase) {
        while((temp = get_trie_node_children(current, *phrase)) == NULL && 
                current != trie_root) {
            current = current->fail;
        }

        if (1 == is_trie_node_root(current)) {
            current = get_trie_node_children(trie_root, *phrase);

            if (current == NULL) {
                current = trie_root;
            }

            temp = current;
        } else {
            current = temp;
        }
    
        while (temp != trie_root) {
            if (1 == is_trie_node_final(temp)) {
                st = temp->syntax_trees;
                while (NULL != st) {
                    printf ("validating\n");
                    validate_tree((struct parse_tree_struct*) st->value, text_match_index);
                    st = st->next;
                }
            }
            temp = temp->fail;
        }
        
        phrase++;
    }

    st = syntax_trees;
    printf ("matching\n");
    if (syntax_trees == NULL) {
        printf("nothing added\n");
    }
    while (st) {
        printf ("in while\n");
        struct parse_tree_struct *t = (struct parse_tree_struct *) st->value;
        if(operator_truth_value(t->val, text_match_index, 1)) {
            printf("Found: %s - %s\n", udid, "TIHS");
        }
        t->truth_value = t->initial_truth_value;
        st = st->next;
    }
    printf ("returning from trie_match\n");
    return 0;
}

