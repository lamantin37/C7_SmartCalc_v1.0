#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_stack.h"

typedef double (*MathFunc)(double);
MathFunc trig_functions[] = {cos, sin, tan, acos, asin, atan, sqrt, log, log10};

#define NUM_OF_L_OP 17
const char *LONG_OPERANDS[] = {"cos",  "sin", "tan", "acos", "asin", "atan",
                               "sqrt", "ln",  "log", "mod",  "+",    "-",
                               "*",    "/",   "^",   "(",    ")"};
const int LONG_OPERANDS_PRIORITY[] = {5, 5, 5, 5, 5, 5, 5, 5, 5,
                                      5, 2, 2, 3, 3, 4, 1, 1};
const double alt_names[] = {0, 1,  2,  3,  4,  5,  6,  7, 8,
                            9, 10, 11, 12, 13, 14, 15, 16};

#define CHECK_L_OP(str)                                                        \
  ({                                                                           \
    int result = -1;                                                           \
    for (int i = 0; i < NUM_OF_L_OP; i++) {                                    \
      result = (strncmp(str, LONG_OPERANDS[i], strlen(LONG_OPERANDS[i])) == 0) \
                   ? i                                                         \
                   : result;                                                   \
    }                                                                          \
    result;                                                                    \
  })

#define CONVERT_STR_TO_NUM(str, endptr)                                        \
  ({                                                                           \
    double result = strtod(str, endptr);                                       \
    result;                                                                    \
  })

#define countLOWop(operator, value1, value2)                                   \
  ({                                                                           \
    typeof(value1) _v1 = (value1);                                             \
    typeof(value2) _v2 = (value2);                                             \
    typeof(_v1) _result;                                                       \
    if (!strncmp((operator), "+", 1)) {                                        \
      _result = _v2 + _v1;                                                     \
    } else if (!strncmp((operator), "-", 1)) {                                 \
      _result = _v2 - _v1;                                                     \
    } else if (!strncmp((operator), "*", 1)) {                                 \
      _result = _v2 * _v1;                                                     \
    } else if (!strncmp((operator), "/", 1)) {                                 \
      _result = _v2 / _v1;                                                     \
    } else if (!strncmp((operator), "^", 1)) {                                 \
      _result = pow(_v2, _v1);                                                 \
    } else if (!strncmp((operator), "mod", 3)) {                               \
      _result = fmod(_v2, _v1);                                                \
    }                                                                          \
    _result;                                                                   \
  })

#define MATH_FUNCTION_RESULT(func, value) (func(value))

void fillStackDijkstra(Stack *stack, const char *expression);
void countValue(Stack *stack);
