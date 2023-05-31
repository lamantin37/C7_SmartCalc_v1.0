#include "s21_smartcalc.h"
#include <string.h>

int main() {
  double result = MATH_FUNCTION_RESULT(tan, 0.8);
  printf("tan(0.8) = %lf\n", result);
  char buf[255] = "1 + sin(4/2) ^ 2";
  // 3 4 2 * 1 5 - 2 ^ / +

  Stack stack1;
  init(&stack1);

  fillStackDijkstra(&stack1, buf);
  for (int i = stack1.top; i >= 0; i--) {
    if (stack1.is_num[i] == 0) {
      printf("%s\n", LONG_OPERANDS[(int)stack1.data[i]]);
    } else {
      printf("%lf\n", stack1.data[i]);
    }
  }

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
        }
        pop(&tmp);
      } else if (tmp.top >= 1 &&
                 (LONG_OPERANDS_PRIORITY[(int)tmp.data[tmp.top]] <=
                  LONG_OPERANDS_PRIORITY[(int)tmp.data[tmp.top - 1]]) &&
                 tmp.data[tmp.top] != 15.0) {
        int priority = LONG_OPERANDS_PRIORITY[(int)tmp.data[tmp.top]];
        pop(&tmp);
        while (tmp.top >= 1 &&
               (LONG_OPERANDS_PRIORITY[(int)tmp.data[tmp.top]] >= priority) &&
               !isEmpty(&tmp)) {
          push(stack, pop(&tmp), 0);
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
  }
}
