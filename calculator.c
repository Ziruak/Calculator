#include "calculator.h"

op parse_op(const char **str) {
    op opr = {INVALID,0.0,0};
    char sym;
    static char last_sym = '(';
    int scanned = 0;
    if (strchr("1234567890.",**str) && sscanf(*str,"%lf%n", &opr.val, &scanned) > 0) {
        opr.type = NUMBER;
        (*str)+=scanned;
        last_sym='0';
    } else if (sscanf(*str,"%c", &sym) > 0) {
        if (strchr("+-*x^()/mcstal", sym)) {
            switch (sym) {
                case '+': 
                if (!strchr("+-*^(/m", last_sym)) {
                opr.type = OP_BIN_PLUS;
                opr.priority = 1;
                } else {
                    opr.type = OP_UN_PLUS;
                    opr.priority = 5;
                }
                (*str)++;
                break;
                case '-':
                if (!strchr("+-*^(/m", last_sym)) {
                opr.type = OP_BIN_MINUS;
                opr.priority = 1;
                } else {
                    opr.type = OP_UN_MINUS;
                    opr.priority = 5;
                }
                (*str)++;
                break;
                case '*': 
                opr.type = OP_BIN_MULT;
                opr.priority = 2;
                (*str)++;
                break;
                case 'x': 
                opr.type = X_VAL;
                (*str)++;
                break;
                case '^': 
                opr.type = OP_BIN_POW;
                opr.priority = 3;
                (*str)++;
                break;
                case '(':
                opr.type = BR_OPEN;
                (*str)++;
                break;
                case ')':
                opr.type = BR_CLOSE;
                (*str)++;
                break;
                case '/':
                opr.type = OP_BIN_DIV;
                opr.priority = 2;
                (*str)++;
                break;
                case 'm':
                sscanf(*str, "mod%n", &scanned);
                 if (scanned == 3) {
                     opr.type = OP_BIN_MOD;
                     opr.priority = 2;
                     (*str)+=3;
                 }
                 break;
                case 'c':
                sscanf(*str, "cos%n", &scanned);
                 if (scanned == 3) {
                     opr.type = F_COS;
                     opr.priority = 4;
                     (*str)+=3;
                 }
                 break;
                 case 's':
                 sscanf(*str, "sin%n", &scanned);
                 if (scanned == 3) {
                     opr.type = F_SIN;
                     opr.priority = 4;
                     (*str)+=3;
                 }
                 sscanf(*str, "sqrt%n", &scanned);
                 if (scanned == 4) {
                     opr.type = F_SQRT;
                     opr.priority = 4;
                     (*str)+=4;
                 } 
                 break;
                case 't':
                sscanf(*str, "tan%n", &scanned);
                if (scanned == 3) {
                     opr.type = F_TAN;
                     opr.priority = 4;
                     (*str)+=3;
                 }
                 break;
                case 'a':
                sscanf(*str, "asin%n", &scanned);
                if (scanned == 4) {
                     opr.type = F_ASIN;
                     opr.priority = 4;
                     (*str)+=4;
                 } 
                 sscanf(*str, "acos%n", &scanned);
                 if (scanned == 4) {
                     opr.type = F_ACOS;
                     opr.priority = 4;
                     (*str)+=4;
                 } 
                 sscanf(*str, "atan%n", &scanned);
                 if (scanned == 4) {
                     opr.type = F_ATAN;
                     opr.priority = 4;
                     (*str)+=4;
                 }
                 break;
                 case 'l':
                 sscanf(*str, "ln%n", &scanned);
                 if (scanned == 2) {
                     opr.type = F_LN;
                     opr.priority = 4;
                     (*str)+=2;
                 } 
                 sscanf(*str, "log%n", &scanned);
                  if (scanned == 3) {
                     opr.type = F_LOG;
                     opr.priority = 4;
                     (*str)+=3;
                 }
                 break;
            }
            last_sym = sym;
        } else if (sym == '\n' || sym == '\0') {
            opr.type = SEQ_END;
            last_sym = '(';
        }
    } else {
        opr.type = SEQ_END;
        last_sym = '(';
    }
    while (**str== ' ') (*str)++;
    return opr;
}

double s21_sum(double a, double b) {
    return a+b;
}

double s21_sub(double a, double b) {
    return a-b;
}

double s21_mul(double a, double b) {
    return a*b;
}

double s21_div(double a, double b) {
    return a/b;
}

int calculate_num(stack *nums, stack *ops) {
    int calculated = 1;
    op cur_op = s_top(ops);
    op_type type = cur_op.type;
    double (*bins[6]) (double a, double b) = {s21_sum,s21_sub,s21_mul,s21_div,fmod,pow};
    double (*funs[9]) (double x) = {sin,cos,tan,acos,asin,atan,sqrt,log,log10};
    s_pop(ops);
    if (type == OP_UN_MINUS) {
        cur_op = s_top(nums);
        calculated = s_pop(nums);
        cur_op.val = -cur_op.val;
    } else if(type == OP_UN_PLUS) {
    } else if (type > OP_BINS && type < OP_END) {
        op temp_op = s_top(nums);
        calculated = s_pop(nums);
        cur_op = s_top(nums);
        calculated = s_pop(nums);
        cur_op.val = bins[type - OP_BINS - 1](cur_op.val, temp_op.val);
    } else if (type > F_START && type < F_END) {
        cur_op = s_top(nums);
        calculated = s_pop(nums);
        cur_op.val = funs[type - F_START - 1](cur_op.val);
    }
    if (calculated) calculated = type;
    if (cur_op.type == NUMBER) s_push(nums, cur_op);
    return calculated;
}

int calculate_seq(stack *nums, stack *ops) {
    op cur = s_top(ops);
    op_type res = INVALID;
    if (cur.type == BR_CLOSE) {
        s_pop(ops);
        while((res = calculate_num(nums,ops)) && res != BR_OPEN);
        if ((cur = s_top(ops)).type > F_START && cur.type < F_END)
            calculate_num(nums,ops);
    }
    return res;
}

double calculate(const char* str, double xval) {
    stack numbers = {NULL};
    stack operators = {NULL};
    op cur;
    int process = 1;
    s_push(&operators, (op){BR_OPEN,0.,0});
    while (process) {
        cur = parse_op(&str);
        if (cur.type == INVALID) {
            process = 0;
            s_push(&numbers,(op){INVALID,S21_NAN,0});
        }
        if (cur.type == NUMBER) {
            s_push(&numbers, cur);
        } else if (cur.type == X_VAL) {
            cur.type = NUMBER;
            cur.val = xval;
            s_push(&numbers, cur);
        } else if (cur.type > OP_START && cur.type < OP_END) {
            if (s_top(&operators).priority >= cur.priority) {
                s_push(&operators, (op){BR_CLOSE,0.0,0});
                calculate_seq(&numbers, &operators);
                s_push(&operators, (op){BR_OPEN,0.0,0});
            }
            s_push(&operators, cur);
        } else if (cur.type == BR_OPEN) {
            s_push(&operators, cur);
        } else if (cur.type == BR_CLOSE) {
            s_push(&operators, cur);
            calculate_seq(&numbers, &operators);
        } else if (cur.type > F_START && cur.type < F_END) {
            s_push(&operators, cur);
        } else if (cur.type == SEQ_END) {
            s_push(&operators, (op){BR_CLOSE,0.0,0});
            calculate_seq(&numbers, &operators);
            process = 0;
        }
    }
    cur = s_top(&numbers);
    s_clear(&numbers);
    s_clear(&operators);
    return cur.val;
}