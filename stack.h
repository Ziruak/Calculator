#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

typedef enum operat_or_and_types {
INVALID = 0,
X_VAL,
NUMBER,  
OP_START,  
OP_UN_PLUS,
OP_UN_MINUS,
OP_BINS,
OP_BIN_PLUS,
OP_BIN_MINUS,
OP_BIN_MULT,
OP_BIN_DIV,
OP_BIN_MOD,
OP_BIN_POW,
OP_END,
BR_OPEN,
BR_CLOSE,
F_START,
F_SIN,
F_COS,
F_TAN,
F_ACOS,
F_ASIN,
F_ATAN,
F_SQRT,
F_LN,
F_LOG,
F_END,
SEQ_END
} op_type;

typedef struct operat_or_and {
    op_type type;
    double val;
    short priority;
} op;

typedef struct s_op_node {
    op el;
    struct s_op_node *next;
} node;

typedef struct stack_op {
    node *_lst;
} stack;

int s_empty(stack *s);

op s_top(stack *s);

int s_pop(stack *s);

void s_push(stack *s, op el);

void s_clear(stack *s);

#endif