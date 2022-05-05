#include "stack.h"

int s_empty(stack *s) {
    return s->_lst==NULL;
}

op s_top(stack *s) {
    op res;
    if (s->_lst) {
        res = s->_lst->el;
    } else {
        res = (op){INVALID, 0.0, 0};
    }
    return res;
}

int s_pop(stack *s) {
    int res = !s_empty(s);
    if (res) {
        node *first = s->_lst;
        s->_lst = s->_lst->next;
        free(first); 
    }
    return res;
}

void s_push(stack *s, op el) {
    node *new_node = (node*)malloc(sizeof(node));
    new_node->next = s->_lst;
    s->_lst = new_node;
    s->_lst->el = el;
}

void s_clear(stack *s) {
    while (s_pop(s));
}