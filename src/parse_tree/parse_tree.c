#include "stdlib.h"
#include "stdio.h"

#include "../stack/stack.h"
#include "../sanitizer/sanitizer.h"
#include "../pair/pair.h"
#include "parse_tree.h"
#include "utils.h"

parse_tree_t *new_parse_tree (void *val, parse_tree_t *left, parse_tree_t *right, short int is_operator) {
    parse_tree_t *t = (parse_tree_t*) malloc (sizeof(parse_tree_t));

    t->val = val;
    t->left = left;
    t->right = right;
    t->is_operator = is_operator;
    t->side = 0;
    t->parent = NULL;

    return t;
}

void set_parse_tree_side (parse_tree_t *node, int side) {
    if (side != 1 || side != 2) {
        side = 0;
    }
    node->side = side;
}

void set_parse_tree_parent (parse_tree_t *node, const parse_tree_t *parent) {
    node->parent = parent;
}

/*
 * construct_tree will return a parse tree for a given formula
 *
 * It receives a formula which is assumed is sanitized and all
 * and a stack of parentheses pairs
 * Both arguments will be altered after this function returns:
 *  - the stack will be empty
 */
parse_tree_t *construct_tree(char *str, stack_t *brackets) {
    // root will be returned
    parse_tree_t *root;

    // These are used internally to construct the tree
    parse_tree_t *left, *right;

    // These are used internally to handle parentheses
    pair_t *outer_pair, *inner_pair, *current_pair;

    // This is used to put the parentheses pair in it from the brackets stack
    // and then are popped and processed
    stack_t *inner_pair_stack = new_stack();

    // This stack is used to construct the tree
    stack_t *children_stack = new_stack();
    
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
                        left = new_parse_tree((void *)token, NULL, NULL, 0);
                    }
                    root = new_parse_tree((void *)'~', left, NULL, 1);
                    set_parse_tree_side(left, PARSE_TREE_LEFT);
                    set_parse_tree_parent(left, root);
                } else {
                    unencode_string(token);
                    root = new_parse_tree((void *)token, NULL, NULL, 0);
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
                        left = new_parse_tree((void *) token, NULL, NULL, 0);
                    }
                    root = new_parse_tree((void*) '~', left, NULL, 1);
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
                        left = new_parse_tree((void *) token, NULL, NULL, 0);
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
                        right = new_parse_tree((void *) token, NULL, NULL, 0);
                    }
                    root = new_parse_tree((void*) '~', right, NULL, 1);
                    set_parse_tree_side(right, PARSE_TREE_RIGHT);
                    set_parse_tree_parent(right, root);
                    right = root;
                } else {
                    // The right part is not negated
                    if (str[current_pair->operator_position + 1] == '[') {
                        right = (parse_tree_t*) stack_pop(children_stack);
                    } else {
                        token = string_copy(str, current_pair->operator_position + 1, current_pair->end - 1);
                        right = new_parse_tree((void *) token, NULL, NULL, 0);
                    }
                }

                root = new_parse_tree((void*) str[current_pair->operator_position], left, right, 1);

                set_parse_tree_side(left, PARSE_TREE_LEFT);
                set_parse_tree_parent(left, root);
                set_parse_tree_side(right, PARSE_TREE_RIGHT);
                set_parse_tree_parent(right, root);

                stack_push(children_stack, (void *) root);
            }
        }
    }
    return (parse_tree_t*)stack_pop(children_stack);
}

