#include <stdlib.h>

#include "pair.h"

inline pair_t *new_pair(int start) {
    pair_t *new_p = (pair_t*) malloc(sizeof(pair_t));
    if (!new_p) {
        exit(19);
    }
    new_p->start = start;
    new_p->end = -1;
    new_p->operator_position = -1;

    return new_p;
}

inline void set_pair_end(pair_t *p, int end) {
    p->end = end;
}

inline void set_pair_op(pair_t *p, int op) {
    p->operator_position = op;
}

