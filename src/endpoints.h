#ifndef ROOX_ENDPOINTS_H
#define ROOX_ENDPOINTS_H

/*
 * Endpoints defines url mappings
 * Each url must provide a callback with two arguments
 *  - first argument is an ID
 *  - second argument is a text
 * 
 * E.g.: for /add, first argument is formula ID and the second is
 *      the formula from which the syntax tree will be constructed
 */
typedef struct endpoint_struct {
    char *uri;
    int (*callback) (char *, char *);
    char **messages;
} endpoint_t;

#endif

