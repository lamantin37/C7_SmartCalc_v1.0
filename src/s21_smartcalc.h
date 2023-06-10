#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <stdarg.h>

#include "s21_stack.h"

#define NUM_OF_L_OP 17
extern double (*trig_functions[])(double);
extern const char *LONG_OPERANDS[];
extern const int LONG_OPERANDS_PRIORITY[];
extern const double alt_names[];

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

#define REPLACE_CHAR(str, find, replace)                                       \
  do {                                                                         \
    char *current = str;                                                       \
    while (*current != '\0') {                                                 \
      if (*current == find) {                                                  \
        *current = replace;                                                    \
      }                                                                        \
      current++;                                                               \
    }                                                                          \
  } while (0)

int fillStackDijkstra(Stack *stack, const char *expression, double value);
double countValue(Stack *stack);
double s21_smartcalc(const char *expression, double value);
