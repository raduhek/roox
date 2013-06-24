#include "stdlib.h"
#include "stdio.h"

#include "../stack/stack.h"
#include "../list/list.h"
#include "../sanitizer/sanitizer.h"
#include "../pair/pair.h"
#include "parse_tree.h"
#include "utils.h"

extern struct stack_struct *phrase_parentheses_stack;
extern struct trie_node_struct *trie_root;
extern list_node_t *syntax_trees;
extern void trie_insert(struct trie_node_struct *, const char *, void *);

parse_tree_t *new_parse_tree (char val,  parse_tree_t *left, parse_tree_t *right, short int is_operator) {
    parse_tree_t *t = (parse_tree_t*) malloc (sizeof(parse_tree_t));

    t->val = val;
    t->UDID = NULL;
    t->left = left;
    t->right = right;
    t->is_operator = is_operator;
    t->side = 0;
    t->parent = NULL;
    t->truth_value = 0;
    t->initial_truth_value = 0;
    t->modifier = -1;

    if (val == '~') {
        t->truth_value = 1 - left->truth_value;
        t->initial_truth_value = t->truth_value;
    }
    return t;
}

void set_parse_tree_side (parse_tree_t *node, int side) {
    if (side != 1 && side != 2) {
        side = 0;
    }
    node->side = side;
}

void set_parse_tree_parent (parse_tree_t *node, parse_tree_t *parent) {
    node->parent = parent;
}

int operator_truth_value(parse_tree_t *t, int switcher, int ws) {
    int truth_value;

    if (ws == 0) {
        truth_value = t->truth_value;
    } else {
        if (t->modifier == switcher) {
            truth_value = t->truth_value;
        } else {
            truth_value = t->initial_truth_value;
        }
    }

    if (t->val == '&') {
        return (truth_value == 3) ? 1: 0;
    }
    if (t->val == '|') {
        return (truth_value | 0) ? 1: 0;
    }
    return truth_value ? 1: 0;
}

/*
 * construct_tree will return a parse tree for a given formula
 *
 * It receives a formula which is assumed is sanitized and all
 * and a stack of parentheses pairs
 * Both arguments will be altered after this function returns:
 *  - the stack will be empty
 */
parse_tree_t *construct_tree(char *str, 
                                stack_rt *brackets,
                                void (*callback)(struct trie_node_struct *root, const char *s, void *data),
                                struct trie_node_struct *trie_root,
                                char *UDID) {
    // root will be returned
    parse_tree_t *root;

    // These are used internally to construct the tree
    parse_tree_t *left, *right;

    // These are used internally to handle parentheses
    pair_t *outer_pair, *inner_pair, *current_pair;

    // This is used to put the parentheses pair in it from the brackets stack
    // and then are popped and processed
    stack_rt *inner_pair_stack = new_stack();

    // This stack is used to construct the tree
    stack_rt *children_stack = new_stack();
    
    char *token;

    while (NULL != (outer_pair = (pair_t*)stack_peak(brackets))) {
        outer_pair = (pair_t*) stack_pop(brackets);
        stack_push (inner_pair_stack, (void*) outer_pair);
        while ((NULL != (inner_pair = (pair_t*) stack_peak(brackets))) &&
                                inner_pair->start > outer_pair->start &&
                                inner_pair->end < outer_pair->end) {

            inner_pair = (pair_t*) stack_pop(brackets);
            stack_push (inner_pair_stack, (void *) inner_pair);
        }

        while (NULL != (current_pair = (pair_t*) stack_pop (inner_pair_stack))) {
            /*
             * We have a few cases to handle here
             */
            /*
                LEFT and RIGHT children
            */
            left = NULL;
            right = NULL;
            str[current_pair->start] = '[';
            str[current_pair->end] = ']';
            // This is the case of a single token in parentheses (maybe negated)
            if (-1 == current_pair->operator_position) {
                token = string_copy(str, current_pair->start + 1, current_pair->end - 1);

                // It's negated :D
                if (token[0] == '~') {
                    token++;
                    
                    if (token[0] == '[') {
                        left = (parse_tree_t*) stack_pop(children_stack);
                    } else {
                        unencode_string(token);
                        left = new_parse_tree(token[0], NULL, NULL, 0);
                        callback(trie_root, token, (void *) left);
                    }

                    root = new_parse_tree('~', left, NULL, 1);
                    set_parse_tree_side(left, PARSE_TREE_LEFT);
                    set_parse_tree_parent(left, root);
                } else {
                    unencode_string(token);
                    root = new_parse_tree(token[0], NULL, NULL, 0);
                    callback(trie_root, token, (void *) root);
                }

                stack_push(children_stack, (void*)root);
            } else {
                // Here we have to check whether we have processed a pair (by checking for [ or ])
                // or maybe a negation sign is somewhere
                // Process left side
                if (str[current_pair->start + 1] == '~') {
                    // The left part is negated
                    if (str[current_pair->start + 2] == '[') {
                        // We have processed this pair, so just get the node
                        left = (parse_tree_t*) stack_pop(children_stack);
                    } else {
                        token = string_copy(str, current_pair->start + 2, current_pair->operator_position - 1);
                        unencode_string(token);
                        left = new_parse_tree(token[0], NULL, NULL, 0);
                        callback(trie_root, token, (void *) left);
                    }
                    root = new_parse_tree('~', left, NULL, 1);
                    set_parse_tree_side(left, PARSE_TREE_LEFT);
                    set_parse_tree_parent(left, root);
                    left = root;
                } else {
                    // The left part is not negated
                    if (str[current_pair->start + 1] == '[') {
                        left = (parse_tree_t*) stack_pop(children_stack);
                    } else {
                        token = string_copy(str, current_pair->start + 1, current_pair->operator_position - 1);
                        unencode_string(token);
                        left = new_parse_tree(token[0], NULL, NULL, 0);
                        callback(trie_root, token, (void *) left);
                    }
                }

                // Process right side
                if (str[current_pair->operator_position + 1] == '~') {
                    // The right part is negated
                    if (str[current_pair->operator_position + 2] == '[') {
                        // We have processed this pair, so just get the node
                        right = (parse_tree_t*) stack_pop(children_stack);
                    } else {
                        token = string_copy(str, current_pair->operator_position + 2, current_pair->end - 1);
                        right = new_parse_tree(token[0], NULL, NULL, 0);
                        callback(trie_root, token, (void *) right);
                    }
                    root = new_parse_tree('~', right, NULL, 1);
                    set_parse_tree_side(right, PARSE_TREE_RIGHT);
                    set_parse_tree_parent(right, root);
                    right = root;
                } else {
                    // The right part is not negated
                    if (str[current_pair->operator_position + 1] == '[') {
                        right = (parse_tree_t*) stack_pop(children_stack);
                    } else {
                        token = string_copy(str, current_pair->operator_position + 1, current_pair->end - 1);
                        right = new_parse_tree(token[0], NULL, NULL, 0);
                        callback(trie_root, token, (void *) right);
                    }
                }

                root = new_parse_tree(str[current_pair->operator_position], left, right, 1);

                root->truth_value = operator_truth_value(left, 0, 0) * 2 + operator_truth_value(right, 0, 0) * 1;
                root->initial_truth_value = root->truth_value;

                set_parse_tree_side(left, PARSE_TREE_LEFT);
                set_parse_tree_parent(left, root);
                set_parse_tree_side(right, PARSE_TREE_RIGHT);
                set_parse_tree_parent(right, root);

                stack_push(children_stack, (void *) root);
            }
        }
    }
    root = (parse_tree_t*) stack_pop(children_stack);
    root->UDID = UDID;
    return root;
}

void validate_tree(parse_tree_t *node, int switcher) {
    parse_tree_t *t = node;

    while (t) {
        t->modifier = switcher;
        switch (t->val) {
            case '|':
            case '&':
                t->truth_value = operator_truth_value(t->left, switcher, 1) * 2 +
                                operator_truth_value(t->right, switcher, 1) * 1;
                break;
            case '~':
                t->truth_value = 1 - operator_truth_value(t->left, switcher, 1);
                break;
            default:
                t->truth_value = 1;
                break;
        }

        t = t->parent;
    }
}

int insert_phrase(char *udid, char *phrase) {
    // @ep_ok = extract_parentheses check variable
    // @bo_ok = boolean_operators check variable
    int ep_ok, bo_ok;

    // @pt_root is the root of a syntax tree
    parse_tree_t *pt_root;


    phrase = remove_whitespaces(phrase, 0);
    remove_empty_parentheses(phrase);
    phrase = remove_whitespaces(phrase, 0);
    surround_with_parentheses(phrase); 

    ep_ok = extract_parentheses(phrase, phrase_parentheses_stack);
    if (0  == ep_ok) {
        printf("WARNING: Could not extract parentheses positions\n");
        // empty_stack(&phrase_parentheses_stack);
        return 1;
    }

    remove_duplicate_parentheses(phrase, phrase_parentheses_stack);
    phrase = remove_whitespaces(phrase, 1);

    bo_ok = check_boolean_operators(phrase);
    if (0 == bo_ok) {
        printf("WARNING: Operators & or | are not binary as required\n");
        
        return 2;
    }

    ep_ok = extract_parentheses(phrase, phrase_parentheses_stack);
        
    if (0  == ep_ok) {
        printf("WARNING: Could not extract parentheses positions\n");
        return 3;
    }

    pt_root = construct_tree(phrase, phrase_parentheses_stack, trie_insert, trie_root, udid); 

    if (pt_root == NULL) {
        printf("ERROR: Could not construct syntax tree from:\n%s\n", phrase);
        // This is a 500
        return 4;
    }

    list_add(&syntax_trees, (void*) pt_root);
    if (syntax_trees == NULL) {
        printf("nothing added... wtf??\n");
    }

    return 0;
}

