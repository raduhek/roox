#ifndef PAIR_H
#define PAIR_H

typedef struct parantheses_match_struct {
    int start;
    int end;
    int operator_position;
} pair_t;

pair_t *new_pair (int);
void set_pair_end(pair_t *, int);
void set_pair_op(pair_t *, int);

#endif

