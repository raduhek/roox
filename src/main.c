#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "trie/trie.h"
#include "list/list.h"
#include "stack/stack.h"
#include "parse_tree/parse_tree.h"
#include "endpoints.h"
#include "http/defines.h"
#include "http/http.h"

endpoint_t endpoints[3];

// @text_match_index is the index of current text that is match (see validate_tree)
int text_match_index = 0;

// @trie_root is the root node of the trie
trie_node_t *trie_root; // = new_trie_node(ROOT_CHAR, NULL, ROOT_ID); 

// @phrase_parentheses_stack is used for processing parentheses pairs
struct stack_struct *phrase_parentheses_stack; // = new_stack();

// @syntax_trees holds all constructed trees 
list_node_t *syntax_trees = NULL; 

void define_endpoints() {
    endpoints[0].uri = "/add";
    endpoints[0].callback = insert_phrase;
    endpoints[0].messages = (char **) malloc(5 * sizeof(char *));
    endpoints[0].messages[0] = "OK";
    endpoints[0].messages[1] = "Could not extract parentheses";
    endpoints[0].messages[2] = "Operators not binary";
    endpoints[0].messages[3] = "Could not extract parentheses";
    endpoints[0].messages[4] = "500: No syntax tree";


    endpoints[1].uri = "/match";
    endpoints[1].callback = trie_match;
    endpoints[0].messages = (char **) malloc(1 * sizeof(char *));
    endpoints[0].messages[0] = "OK";
}


int main(int argc, char* argv[])
{
    int i; // index variable name used by Herodot and others... old

    struct sockaddr clientaddr;
    socklen_t addrlen;

    char arg_letter;    
    
    char PORT[6] = "5004";
    int slot = 0;


    //Parsing the command line arguments
    while ((arg_letter = getopt (argc, argv, "p:")) != -1)
        switch (arg_letter)
        {
            case 'p':
                strcpy(PORT,optarg);
                break;
            case '?':
                fprintf(stderr,"Wrong arguments given!!!\n");
                exit(1);
            default:
                exit(1);
        }
    
    for (i = CONNMAX - 1; i >= 0; --i) {
        clients[i] = -1;
    }

    start_server(PORT);
    define_endpoints();

    /*
        At this point the server is up, so init global variables
    */
    trie_root = new_trie_node(ROOT_CHAR, NULL, ROOT_ID); 
    phrase_parentheses_stack = new_stack();

    // ACCEPT connections
    while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[slot]<0)
            perror ("accept() error");
        else
        {
            if ( fork()==0 )
            {
                // Respond pretty much deals with everything that needs to be done
                // for any endpoint
                respond(slot, endpoints, 2);
                exit(0);
            }
        }

        // Find next free slot
        while (clients[slot] != -1) {
            slot = (slot+1) % CONNMAX;
        }
    }

    return 0;
}

void print_found(char *text, char *id) {
    printf("FOUND: %s -> %s", text, id);
}

/*void print_tree(parse_tree_t *t, int l) {
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
}*/
/*
int main2(int argc, char **argv) {
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
*/
