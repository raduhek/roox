#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "../stack/stack_node.h"

#define PARSE_TREE_LEFT 2
#define PARSE_TREE_RIGHT 1

/*
 * Definition of parse_tree_struct
 */
typedef struct parse_tree_struct {
    void *val;
    
    struct parse_tree_struct *parent;
    struct parse_tree_struct *left;
    struct parse_tree_struct *right;

    // side tells whether is either left or right
    // child
    short int side;

    short int is_operator;
} parse_tree_t;

parse_tree_t *new_parse_tree (void *, parse_tree_t *, parse_tree_t *, short int);
void set_parse_tree_side (parse_tree_t *, int);
void set_parse_tree_parent (parse_tree_t *, const parse_tree_t *);
parse_tree_t *construct_tree (char *, stack_t *); 
#endif

