#include <check.h>
#include <stdbool.h>

#include "s21_smartcalc.h"

const char *LONG_OPERANDS_TEST[] = {"cos(x)",  "sin(x)",  "tan(x)",
                                    "acos(x)", "asin(x)", "atan(x)",
                                    "sqrt(x)", "ln(x)",   "log(x)"};

const char *LONG_OPERANDS_TEST_BIAS[] = {"+", "-", "*", "/", "^"};

#define trig_func_test(func, str, value)                                 \
  ({                                                                     \
    double math_res = func(value);                                       \
    int num = 0;                                                         \
    double smartcalc_res = 0.f;                                          \
    int ret = s21_smartcalc(str, value, &smartcalc_res, &num);           \
    if (!isnan(math_res) && !isnan(smartcalc_res) && !isinf(math_res) && \
        !isinf(smartcalc_res)) {                                         \
      ck_assert_double_eq(math_res, smartcalc_res);                      \
    }                                                                    \
  })

#define bias_op_test(str, value)                               \
  ({                                                           \
    char buf[256] = "\0";                                      \
    sprintf(buf, "%lf %s %lf", value, str, value);             \
    int num = 0;                                               \
    double smartcalc_res = 0.f;                                \
    int ret = s21_smartcalc(buf, 0, &smartcalc_res, &num);     \
    double res = countLOWop(str, value, value);                \
    if (!isnan(res) && !isnan(smartcalc_res) && !isinf(res) && \
        !isinf(smartcalc_res)) {                               \
      ck_assert_double_eq(res, smartcalc_res);                 \
    }                                                          \
  })

START_TEST(test1) {
  double step = 0.01;
  for (double x = -100; x <= 100; x += step) {
    for (int i = 0; i <= 8; i++) {
      trig_func_test(trig_functions[i], LONG_OPERANDS_TEST[i], x);
    }
  }
}
END_TEST

START_TEST(test2) {
  double step = 0.5;
  for (double x = -10; x <= 10; x += step) {
    for (int i = 0; i <= 4; i++) {
      bias_op_test(LONG_OPERANDS_TEST_BIAS[i], x);
    }
  }
}
END_TEST

START_TEST(test3) {
  char buf[256] = "10 ****** 10";
  double res = 0.f;
  int num = 0;
  int ret = s21_smartcalc(buf, 0, &res, &num);
  ck_assert_int_eq(ret, -3);

  sprintf(buf, "10 * sin(1))");
  ret = s21_smartcalc(buf, 0, &res, &num);
  ck_assert_int_eq(ret, -3);

  sprintf(buf, "10 ^^ 2");
  ret = s21_smartcalc(buf, 0, &res, &num);
  ck_assert_int_eq(ret, -3);

  sprintf(buf, "((2)");
  ret = s21_smartcalc(buf, 0, &res, &num);
  ck_assert_int_eq(ret, -3);

  sprintf(buf, "5^^2");
  ret = s21_smartcalc(buf, 0, &res, &num);
  ck_assert_int_eq(ret, -3);
}
END_TEST

START_TEST(test4) {
  char buf[256] = "sin(x)^2 + cos(x)^2";
  double res = 0.f;
  int num = 0;
  int ret = s21_smartcalc(buf, 1, &res, &num);
  ck_assert_double_eq(res, 1.f);

  sprintf(buf, "sin(2) + sqrt((4 ^ 2) + (3 ^ 2))");
  ret = s21_smartcalc(buf, 0, &res, &num);
  double math_res = sin(2) + sqrt((4 * 4) + (3 * 3));
  ck_assert_double_eq(res, math_res);

  sprintf(buf, "-5 * (-2 + 7)");
  ret = s21_smartcalc(buf, 0, &res, &num);
  math_res = -5 * (-2 + 7);
  ck_assert_double_eq(res, math_res);

  sprintf(buf, "((5 - 2) * (8 + 4)) / 3");
  ret = s21_smartcalc(buf, 0, &res, &num);
  math_res = ((5 - 2) * (8 + 4)) / 3;
  ck_assert_double_eq(res, math_res);

  sprintf(buf, "((2 + sin(0.5)) * cos(4)) / sqrt(9)");
  ret = s21_smartcalc(buf, 0, &res, &num);
  math_res = ((2 + sin(0.5)) * cos(4)) / sqrt(9);
  ck_assert_double_eq(res, math_res);

  sprintf(buf, "((((1 + 2) * (3 - 4)) + (5 - 3)) * 4)");
  ret = s21_smartcalc(buf, 0, &res, &num);
  math_res = ((((1 + 2) * (3 - 4)) + (5 - 3)) * 4);
  ck_assert_double_eq(res, math_res);

  sprintf(buf, "2^(2^(2^(2)))");
  ret = s21_smartcalc(buf, 0, &res, &num);
  ck_assert_double_eq(res, 65536.f);
}
END_TEST

Suite *Create_suite_s21_trig_test() {
  Suite *suite = suite_create("trig tests");
  TCase *tcase_core = tcase_create("Core");
  tcase_add_test(tcase_core, test1);
  suite_add_tcase(suite, tcase_core);
  return suite;
}

Suite *Create_suite_s21_bias_test() {
  Suite *suite = suite_create("bias tests");
  TCase *tcase_core = tcase_create("Core");
  tcase_add_test(tcase_core, test2);
  suite_add_tcase(suite, tcase_core);
  return suite;
}

Suite *Create_suite_error_test() {
  Suite *suite = suite_create("error tests");
  TCase *tcase_core = tcase_create("Core");
  tcase_add_test(tcase_core, test3);
  suite_add_tcase(suite, tcase_core);
  return suite;
}

Suite *Create_suite_hard_test() {
  Suite *suite = suite_create("hard tests");
  TCase *tcase_core = tcase_create("Core");
  tcase_add_test(tcase_core, test4);
  suite_add_tcase(suite, tcase_core);
  return suite;
}

int main() {
  printf("\n");
  bool exit_code = EXIT_SUCCESS;

  Suite *suite1 = Create_suite_s21_trig_test();
  SRunner *suite_runner1 = srunner_create(suite1);
  srunner_run_all(suite_runner1, CK_NORMAL);
  int failed_count1 = srunner_ntests_failed(suite_runner1);
  srunner_free(suite_runner1);
  if (failed_count1 != 0) {
    printf("Failed tests: %d\n", failed_count1);
    exit_code = EXIT_FAILURE;
  } else {
    printf("Success\n");
  }
  printf("\n");

  exit_code = EXIT_SUCCESS;

  Suite *suite2 = Create_suite_s21_bias_test();
  SRunner *suite_runner2 = srunner_create(suite2);
  srunner_run_all(suite_runner2, CK_NORMAL);
  failed_count1 = srunner_ntests_failed(suite_runner2);
  srunner_free(suite_runner2);
  if (failed_count1 != 0) {
    printf("Failed tests: %d\n", failed_count1);
    exit_code = EXIT_FAILURE;
  } else {
    printf("Success\n");
  }
  printf("\n");

  exit_code = EXIT_SUCCESS;

  Suite *suite3 = Create_suite_error_test();
  SRunner *suite_runner3 = srunner_create(suite3);
  srunner_run_all(suite_runner3, CK_NORMAL);
  failed_count1 = srunner_ntests_failed(suite_runner3);
  srunner_free(suite_runner3);
  if (failed_count1 != 0) {
    printf("Failed tests: %d\n", failed_count1);
    exit_code = EXIT_FAILURE;
  } else {
    printf("Success\n");
  }
  printf("\n");

  Suite *suite4 = Create_suite_hard_test();
  SRunner *suite_runner4 = srunner_create(suite4);
  srunner_run_all(suite_runner4, CK_NORMAL);
  failed_count1 = srunner_ntests_failed(suite_runner4);
  srunner_free(suite_runner4);
  if (failed_count1 != 0) {
    printf("Failed tests: %d\n", failed_count1);
    exit_code = EXIT_FAILURE;
  } else {
    printf("Success\n");
  }
  printf("\n");
  return exit_code;
}
