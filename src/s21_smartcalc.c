#include "s21_smartcalc.h"

double (*trig_functions[])(double) = {cos,  sin,  tan, acos, asin,
                                      atan, sqrt, log, log10};
const char *LONG_OPERANDS[] = {"cos",  "sin", "tan", "acos", "asin", "atan",
                               "sqrt", "ln",  "log", "mod",  "+",    "-",
                               "*",    "/",   "^",   "(",    ")"};
const int LONG_OPERANDS_PRIORITY[] = {5, 5, 5, 5, 5, 5, 5, 5, 5,
                                      5, 2, 2, 3, 3, 4, 1, 1};
const double alt_names[] = {0, 1,  2,  3,  4,  5,  6,  7, 8,
                            9, 10, 11, 12, 13, 14, 15, 16};

double s21_smartcalc(const char *expression, double value) {
  Stack stack1;
  init(&stack1);
  fillStackDijkstra(&stack1, expression, value);
  return pop(&stack1);
}

void fillStackDijkstra(Stack *stack, const char *expression, double value) {
  Stack tmp;
  init(&tmp);
  for (const char *p = expression; *p != '\0'; p++) {
    int ret = -1;
    if ((ret = CHECK_L_OP(p)) != -1) {
      p += (strlen(LONG_OPERANDS[ret]) - 1);
      push(&tmp, alt_names[ret], 0);
      if (tmp.data[tmp.top] == 16.0) {
        pop(&tmp);
        while (tmp.data[tmp.top] != 15.0) {
          push(stack, pop(&tmp), 0);
          countValue(stack);
        }
        pop(&tmp);
      } else if ((LONG_OPERANDS_PRIORITY[(int)tmp.data[tmp.top]] <=
                  LONG_OPERANDS_PRIORITY[(int)tmp.data[tmp.top - 1]]) &&
                 tmp.data[tmp.top] != 15.0) {
        int priority = LONG_OPERANDS_PRIORITY[(int)tmp.data[tmp.top]];
        pop(&tmp);
        while ((LONG_OPERANDS_PRIORITY[(int)tmp.data[tmp.top]] >= priority) &&
               !isEmpty(&tmp)) {
          push(stack, pop(&tmp), 0);
          countValue(stack);
        }
        push(&tmp, alt_names[ret], 0);
      }
    } else if (isdigit(*p)) {
      char *endptr;
      double num = CONVERT_STR_TO_NUM(p, &endptr);
      push(stack, num, 1);
      p = --endptr;
    } else if (isalpha(*p)) {
      push(stack, value, 1);
    }
  }

  while (!isEmpty(&tmp)) {
    double num = pop(&tmp);
    push(stack, num, 0);
    countValue(stack);
  }
}

void countValue(Stack *stack) {
  double alt_name_op = pop(stack);
  if (0 <= alt_name_op && alt_name_op <= 8) {
    double res =
        MATH_FUNCTION_RESULT(trig_functions[(int)alt_name_op], pop(stack));
    push(stack, res, 1);
  } else {
    double res =
        countLOWop(LONG_OPERANDS[(int)alt_name_op], pop(stack), pop(stack));
    push(stack, res, 1);
  }
}
