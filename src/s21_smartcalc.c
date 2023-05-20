#include "s21_smartcalc.h"

int main() {
  char buf[255] = "0.1e-5 + (3 + sin(8/2 - 3.5)) *2 + x";
  for (char *p = buf; *p != '\0'; p++) {
    int ret = -1;
    if ((ret = CHECK_L_OP(p)) != -1) {
      printf("operand found: %s\n", LONG_OPEDANDS[ret]);
      p += (LONG_OPEDANDS_LEN[ret] - 1);
    } else if (isdigit(*p)) {
      char *endptr;
      double num = CONVERT_STR_TO_NUM(p, &endptr);
      printf("num found: %lf\n", num);
      p = --endptr;
    } else if (isalpha(*p)) {
      printf("variable found: %c\n", *p);
    }
  }
  return 0;
}