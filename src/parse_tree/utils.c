/*
 *  This file contains utility functions needed to input for parse tree
 *
 *  Author ep0
 */

#include <stdio.h>

#include "utils.h"
#include "parse_tree.h"
#include "../stack/stack.h"
#include "../pair/pair.h"

/*
 * Copy a string from start to end. The indexes are inclusive,
 * that is (0, 1) are two characters
 */
char *string_copy(const char *s, int start, int end) {
    if (end < start) {
        return NULL;
    }
    int i;
    char *ret = (char *)malloc((end - start + 2) * sizeof(char));
    ret[end - start + 1] = '\0';
    for (i = end; i >= start; --i) {
        ret[i - start] = s[i];
    }

    return ret;
}

/*
 * extract_parentheses adds in pairs_stack pair_t structures
 * which contain the start and end positions of parentheses pair
 * and also the position of the operator from in between the parentheses
 *
 * It returns 0 if there are more that two Boolean operators in a pair,
 * or 1 otherwise
 *
 */
int extract_parentheses(const char *str, stack_t *pairs_stack) {
    // Used for internal processing
    stack_t *temp_stack = new_stack();

    // Used for handling pairs of parentheses
    pair_t *pair;

    int i = 0;
    int operator_position = -1;
    while (str[i] != '\0') {
        /*
         * When reaching an opening parenthesis, a new pair
         * is pushed into the stack with a starting position
         */
        if (str[i] == '(') {
            pair = new_pair(i);
            stack_push(temp_stack, (void*)pair);
            operator_position = -1;
        }
        if (str[i] == '&' || str[i] == '|') {
            // This means we previously found a Boolean operator in
            // this pair
            if (-1 != operator_position) {
                empty_stack(pairs_stack);
                empty_stack(temp_stack);
                return 0;
            }
            pair = (pair_t*) stack_pop(temp_stack);
            if (NULL == pair) {
                // For an yet unknown reason, this happens
                empty_stack(pairs_stack);
                empty_stack(temp_stack);
                return 0;
            }
            set_pair_op(pair, i);
            operator_position = i;
            stack_push(temp_stack, (void*) pair);
        }

        /*
         * When a closing parenthesis is found, the most recent pushed
         * pair is popped, the end position is set and the updated pair
         * is pushed in pairs_stack
         */
        if (str[i] == ')') {
            pair = (pair_t*) stack_pop(temp_stack);
            set_pair_end(pair, i);
            pair->end = i;
            stack_push(pairs_stack, (void*)pair);
            operator_position = -1;
        }
        i++;
    }
    return 1;
}

