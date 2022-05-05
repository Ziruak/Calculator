#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "stack.h"

/*
* PROGRAM STRUCTURE
* STRING INPUT (GUI PART)
* = PRESSED
* CHECK FOR X: x?[a]:[b];
* [a] ask for x input or graph parameters then [b]
* [b] output result
*/

#define S21_NAN 0./0.

// Calculates infix formula in str and returns it
// if x not in str, xval can be S21_NAN
double calculate(const char* str, double xval);

#endif // !CALCULATOR_H