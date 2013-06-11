#include <stdio.h>

#include "list/list.h"
#include "trie/trie_node.h"
#include "trie/trie.h"
#include "stack/stack.h"
#include "pair/pair.h"
#include "parse_tree/utils.h"
#include "parse_tree/parse_tree.h"
#include "sanitizer/sanitizer.h"

#define INPUT_FILE "phrases"
void print_found(char *id) {
    printf("FOUND IT: %s\n", id);
}

int main(int argc, char **argv) {
    // @line holds a file entry
    char line[2048];
    char udid_entry[32];
    char *udid;

    // @phrase is the processed formula
    char *phrase;

    // file pointers
    FILE *formulas_file, *texts_file;

    // @ep_ok = extract_parentheses check variable
    // @bo_ok = boolean_operators check variable
    int ep_ok, bo_ok;

    // @pt_root is the root of a syntax tree
    parse_tree_t *pt_root;

    // @trie_root is the root node of the trie
    trie_node_t *trie_root = new_trie_node(ROOT_CHAR, NULL, ROOT_ID); 

    // @phrase_parentheses_stack is used for processing parentheses pairs
    stack_t *phrase_parentheses_stack = new_stack();

    // @syntax_trees holds all constructed trees 
    list_node_t *syntax_trees = NULL; 
    // @temp_st will be used to reset syntax trees
    list_node_t *temp_st;

    if (argc != 3) {
        printf("Usage: ./%s [formulas_file] [texts_file]\n", argv[0]);
        exit(0);
    } else {
        formulas_file = freopen (argv[1], "r", stdin);
    }

    if (formulas_file == NULL) {
        perror("Could not open formulas file");
        exit(1);
    }

    
    while (fgets(udid_entry, sizeof(udid_entry), formulas_file)) {
        udid = string_copy(udid_entry, 0, 32);

        fgets(line, sizeof(line), formulas_file);

        sscanf(line, "%[^\r\n]", line);
        if (0 == check_parentheses_closing(line)) {
            printf("WARNING: Improperly closed parentheses\n");
            continue;
        }
        phrase = remove_whitespaces(line, 0);
        remove_empty_parentheses(phrase);
        phrase = remove_whitespaces(phrase, 0);
        surround_with_parentheses(phrase); 
        ep_ok = extract_parentheses(phrase, phrase_parentheses_stack);
        if (0  == ep_ok) {
            printf("WARNING: Could not extract parentheses positions\n");
            continue;
        }
        remove_duplicate_parentheses(phrase, phrase_parentheses_stack);
        phrase = remove_whitespaces(phrase, 1);

        bo_ok = check_boolean_operators(phrase);
        if (0 == bo_ok) {
            printf("WARNING: Operators & or | are not binary as required\n");
            continue;
        }

        ep_ok = extract_parentheses(phrase, phrase_parentheses_stack);
        
        if (0  == ep_ok) {
            printf("WARNING: Could not extract parentheses positions\n");
            continue;
        }

        pt_root = construct_tree(phrase, phrase_parentheses_stack, trie_insert, trie_root, udid); 
        if (pt_root == NULL) {
            printf("ERROR: Could not construct syntax tree from:\n -> %s\n which has been converted to: \n%s\n", line, phrase);
            exit(2);
        }

        list_add(&syntax_trees, (void*) pt_root);

    }

    trie_compile(trie_root);

    texts_file = freopen (argv[2], "r", stdin);
    if (texts_file == NULL) {
        perror("Could not open texts file");
        exit(1);
    }

    while (fgets(line, sizeof(line), texts_file)) {
        sscanf(line, "%[^\r\n]", line);
        trie_match (trie_root, line, print_found);

        temp_st = syntax_trees;
        while (temp_st) {
            reset_parse_tree((parse_tree_t*) temp_st->value);
            temp_st = temp_st->next;
        }
    }

    return 0;
}

