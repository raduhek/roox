#ifndef PT_UTILS_H
#define PT_UTILS_H

#include "../stack/stack.h"

char *string_copy(const char *, int, int);
int extract_parentheses(const char *str, stack_t *pairs_stack); 

#endif

