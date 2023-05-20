#include "s21_smartcalc.h"

int main() {
  char buf[255] = "0.1e-5 + (3 + sin(8/2 - 3.5)) *2";
  for (char *p = buf; *p != '\0'; p++) {
    int ret = -1;
    if ((ret = CHECK_L_OP(p)) != -1) {
      printf("SOMETHING FOUND: %s\n", LONG_OPEDANDS[ret]);
      p += (LONG_OPEDANDS_LEN[ret] - 1);
    } else if (*p >= '0' && *p <= '9') {
      char *endptr;
      double num = CONVERT_STR_TO_NUM(p, &endptr);
      printf("num found: %lf\n", num);
      p = --endptr;
    }
  }
  return 0;
}