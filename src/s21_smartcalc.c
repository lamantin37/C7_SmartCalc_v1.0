#include "s21_smartcalc.h"

int main() {
  char buf[255] = "sin(1) + cos(1)";

  Stack stack1;
  init(&stack1);

  fillStackDijkstra(&stack1, buf);
  double result = pop(&stack1);
  printf("%lf\n", result);

  return 0;
}

void fillStackDijkstra(Stack *stack, const char *expression) {
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
      printf("Variable found: %c\n", *p);
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
