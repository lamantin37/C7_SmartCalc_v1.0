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

int s21_smartcalc(const char *expression, double value, double *res,
                  int *number_of_vars) {
  Stack stack1;
  init(&stack1);
  int ret = fillStackDijkstra(&stack1, expression, value, number_of_vars);
  ret = ret == 0 ? (stack1.top == 0 ? ret : -3) : ret;

  if (ret == -1) {
    *res = NAN;
  }

  if (ret == -2) {
    *res = INFINITY;
  }

  if (ret == 0 && stack1.top == 0) {
    *res = pop(&stack1);
  }

  return ret;
}

int fillStackDijkstra(Stack *stack, const char *expression, double value,
                      int *number_of_vars) {
  Stack tmp;
  init(&tmp);
  int sign = 0;
  int minus = 1;
  sign = *expression == '-' ? 1 : 0;
  for (const char *p = expression; *p != '\0'; p++) {
    int ret = -1;
    if ((ret = CHECK_L_OP(p)) != -1) {
      if (sign == 1 && ret == 11) {
        minus = -1;
        sign = 0;
        continue;
      }
      p += (strlen(LONG_OPERANDS[ret]) - 1);
      push(&tmp, alt_names[ret], 0);
      if (tmp.data[tmp.top] == 16.0) {
        pop(&tmp);
        while (tmp.data[tmp.top] != 15.0) {
          push(stack, pop(&tmp), 0);
          double res = 0.f;
          if (countValue(stack, &res) == 1) {
            return -3;
          }
          if (isnan(res)) {
            return -1;
          } else if (isinf(res)) {
            return -2;
          }
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
          double res = 0.f;
          if (countValue(stack, &res) == 1) {
            return -3;
          }
          if (isnan(res)) {
            return -1;
          } else if (isinf(res)) {
            return -2;
          }
        }
        push(&tmp, alt_names[ret], 0);
      }
      sign = 1;
    } else if (isdigit(*p)) {
      sign = 0;
      char *endptr;
      double num = CONVERT_STR_TO_NUM(p, &endptr);
      num *= minus == -1 ? minus : 1;
      minus = minus == -1 ? 1 : minus;
      push(stack, num, 1);
      p = --endptr;
    } else if (isalpha(*p)) {
      (*number_of_vars)++;
      sign = 0;
      double num = value;
      num *= minus == -1 ? minus : 1;
      minus = minus == -1 ? 1 : minus;
      push(stack, num, 1);
    }
  }

  while (!isEmpty(&tmp)) {
    double num = pop(&tmp);
    push(stack, num, 0);
    double res = 0.f;
    if (countValue(stack, &res) == 1) {
      return -3;
    }
    if (isnan(res)) {
      return -1;
    } else if (isinf(res)) {
      return -2;
    }
  }
  return 0;
}

int countValue(Stack *stack, double *res) {
  double alt_name_op = pop(stack);
  if (0 <= alt_name_op && alt_name_op <= 8 && stack->top >= 0) {
    *res = MATH_FUNCTION_RESULT(trig_functions[(int)alt_name_op], pop(stack));
    push(stack, *res, 1);
  } else if (alt_name_op > 8 && stack->top >= 1) {
    *res = countLOWop(LONG_OPERANDS[(int)alt_name_op], pop(stack), pop(stack));
    push(stack, *res, 1);
  } else {
    return 1;
  }
  return 0;
}
