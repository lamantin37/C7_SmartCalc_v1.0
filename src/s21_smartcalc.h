#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_L_OP 18
const char *LONG_OPEDANDS[] = {"mod",  "cos",  "sin",  "mod", "tan", "acos",
                               "asin", "atan", "sqrt", "ln",  "log", "+",
                               "-",    "*",    "/",    "^",   "(",   ")"};
const int LONG_OPEDANDS_LEN[] = {3, 3, 3, 3, 3, 4, 4, 4, 4,
                                 2, 3, 1, 1, 1, 1, 1, 1, 1};

#define CHECK_L_OP(str)                                                        \
  ({                                                                           \
    int result = -1;                                                           \
    for (int i = 0; i < NUM_OF_L_OP; i++) {                                    \
      result = (strncmp(str, LONG_OPEDANDS[i], LONG_OPEDANDS_LEN[i]) == 0)     \
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

void check_lexeme(char *str, int n);