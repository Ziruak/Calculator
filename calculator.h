#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "stack.h"

#define S21_NAN 0./0.

// Calculates infix formula in str
// if formula is invalid returns nan
// if formula contains x, uses xval instead of it
// if formula correct returns its calculated value
double calculate(const char* str, double xval);

#endif // !CALCULATOR_H