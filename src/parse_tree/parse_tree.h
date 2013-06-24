#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "../stack/stack_node.h"

#define PARSE_TREE_LEFT 2
#define PARSE_TREE_RIGHT 1

struct trie_node_struct;

/*
 * Definition of parse_tree_struct
 */
typedef struct parse_tree_struct {
    char val;
    char *UDID;
    
    struct parse_tree_struct *parent;
    struct parse_tree_struct *left;
    struct parse_tree_struct *right;

    // side tells whether is either left or right
    // child
    short int side;

    short int is_operator;
    short int initial_truth_value;
    short int truth_value;
    short int modifier;
} parse_tree_t;

/*parse_tree_t *new_parse_tree (char, parse_tree_t *, parse_tree_t *, short int);
void set_parse_tree_side (parse_tree_t *, int);
void set_parse_tree_parent (parse_tree_t *, parse_tree_t *);
parse_tree_t *construct_tree (char *, stack_rt *, void (*)(struct trie_node_struct *, const char *, void *), struct trie_node_struct *, char *);
void validate_tree(parse_tree_t *, short int);
void reset_parse_tree(parse_tree_t *r);
int operator_truth_value(parse_tree_t *, int, int);*/

int insert_phrase(char *uid, char *phrase);

#endif

