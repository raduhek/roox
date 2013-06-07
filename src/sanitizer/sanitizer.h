#ifndef SANITIZER_H
#define SANITIZER_H

#include "../stack/stack.h"

int check_parentheses_closing(const char *);
char *remove_whitespaces(const char *, int);
void remove_empty_parentheses(char *);
void surround_with_parentheses(char *);
void unencode_string(char *);
int check_boolean_operators(const char *);
void remove_duplicate_parentheses(char *, stack_t *);



#endif

