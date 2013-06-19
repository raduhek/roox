#include <stdio.h>

#include "list/list.h"
#include "trie/trie_node.h"
#include "trie/trie.h"
#include "stack/stack.h"
#include "pair/pair.h"
#include "parse_tree/utils.h"
#include "parse_tree/parse_tree.h"
#include "sanitizer/sanitizer.h"

void print_found(char *text, char *id) {
    printf("FOUND: %s -> %s", text, id);
}

void read_file(char **buffer, char *file_name) {
    long fsize;
    FILE *fin;
    free(*buffer);
    fin = fopen(file_name, "r");
    // If file could not be opened, buffer will be null
    if (! fin) {
        return;
    }

    // Get file size
    fseek(fin, 0L, SEEK_END);
    fsize = ftell(fin);
    rewind(fin);

    *buffer = (char*) malloc(fsize * sizeof(char));

    if (*buffer == NULL) {
        printf("WARNING: file %s too big: %ld\n", file_name, fsize);
        fclose(fin);
        return;
    }

    if (1 != fread(*buffer, fsize, 1, fin)) {
        printf("WARNING: could not read contents of %s\n", file_name);
        free(*buffer);
        fclose(fin);
        return;
    }

    return;
}

void print_tree(parse_tree_t *t, int l) {
int i;
    if (t == NULL) {
        return;
    }
    print_tree(t->right, l+1);
    for (i = 0; i < l; ++i) {
        printf("\t");
    }
    printf ("%c (%d)\n", t->val, t->truth_value);
    print_tree(t->left, l+1);
}

int main(int argc, char **argv) {
    // @line holds a file entry
    char line[2048];
    // @udid is used to identify a formula
    char udid_entry[32];
    char *udid;

    // @file_buffer holds the contents of a file
    char *file_buffer;

    // @phrase is the processed formula
    char *phrase;

    // file pointers
    FILE *formulas_file;

    // @ep_ok = extract_parentheses check variable
    // @bo_ok = boolean_operators check variable
    int ep_ok, bo_ok;

    // @text_idx is used to read text files
    int text_idx;

    // This is used to not reset trees anymore,
    // but know what value to take depending
    // on what text modified the derivation tree
    short int text_switch;

    short int tree_tv;

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

    if (argc < 2) {
        printf("Usage: ./%s formulas_file text_file [text_file]\n", argv[0]);
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

    temp_st = syntax_trees;
    while (temp_st) {
        parse_tree_t *t = (parse_tree_t *) temp_st->value;
        printf("%s\n", t->UDID);
        print_tree(t, 0);
        printf("\n---------------------------------------------\n");
        temp_st = temp_st->next;
    }

    text_switch = 0;

    for (text_idx = 2; text_idx < argc; ++text_idx) {
        text_switch = 1 - text_switch;

        read_file(&file_buffer, argv[text_idx]);
        // warnings are printed in read_file
        if (NULL == file_buffer) continue;

        // Match formulas
        trie_match (trie_root, file_buffer, text_switch, validate_tree);
        
        // Check syntax trees for root truth values
        temp_st = syntax_trees;
        while (temp_st) {
            parse_tree_t *t = (parse_tree_t*) temp_st->value;

            tree_tv = operator_truth_value(t, text_switch, 1);
            
            t->truth_value = t->initial_truth_value;

            if (tree_tv) {
                print_found(argv[text_idx], t->UDID);
            }

            temp_st = temp_st->next;
        }
    }

    return 0;
}

