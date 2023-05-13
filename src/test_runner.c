#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Suite *Create_suite_s21_add();
Suite *Create_suite_s21_sub();
Suite *Create_suite_s21_is_less();
Suite *Create_suite_s21_is_less_or_equal();
Suite *Create_suite_s21_is_greater();
Suite *Create_suite_s21_is_greater_or_equal();
Suite *Create_suite_s21_is_equal();
Suite *Create_suite_s21_is_not_equal();

int main(void) {
  printf("\n");
  bool exit_code = EXIT_SUCCESS;

  Suite *suite1 = Create_suite_s21_add();
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

  Suite *suite2 = Create_suite_s21_sub();
  SRunner *suite_runner2 = srunner_create(suite2);
  srunner_run_all(suite_runner2, CK_NORMAL);
  int failed_count2 = srunner_ntests_failed(suite_runner2);
  srunner_free(suite_runner2);

  if (failed_count2 != 0) {
    printf("Failed tests: %d\n", failed_count2);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite3 = Create_suite_s21_is_less();
  SRunner *suite_runner3 = srunner_create(suite3);
  srunner_run_all(suite_runner3, CK_NORMAL);
  int failed_count3 = srunner_ntests_failed(suite_runner3);
  srunner_free(suite_runner3);

  if (failed_count3 != 0) {
    printf("Failed tests: %d\n", failed_count3);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite4 = Create_suite_s21_is_less_or_equal();
  SRunner *suite_runner4 = srunner_create(suite4);
  srunner_run_all(suite_runner4, CK_NORMAL);
  int failed_count4 = srunner_ntests_failed(suite_runner4);
  srunner_free(suite_runner4);

  if (failed_count4 != 0) {
    printf("Failed tests: %d\n", failed_count4);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite5 = Create_suite_s21_is_greater();
  SRunner *suite_runner5 = srunner_create(suite5);
  srunner_run_all(suite_runner5, CK_NORMAL);
  int failed_count5 = srunner_ntests_failed(suite_runner5);
  srunner_free(suite_runner5);

  if (failed_count5 != 0) {
    printf("Failed tests: %d\n", failed_count5);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite6 = Create_suite_s21_is_greater_or_equal();
  SRunner *suite_runner6 = srunner_create(suite6);
  srunner_run_all(suite_runner6, CK_NORMAL);
  int failed_count6 = srunner_ntests_failed(suite_runner6);
  srunner_free(suite_runner6);

  if (failed_count6 != 0) {
    printf("Failed tests: %d\n", failed_count6);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite7 = Create_suite_s21_is_equal();
  SRunner *suite_runner7 = srunner_create(suite7);
  srunner_run_all(suite_runner7, CK_NORMAL);
  int failed_count7 = srunner_ntests_failed(suite_runner7);
  srunner_free(suite_runner7);

  if (failed_count7 != 0) {
    printf("Failed tests: %d\n", failed_count7);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite8 = Create_suite_s21_is_not_equal();
  SRunner *suite_runner8 = srunner_create(suite8);
  srunner_run_all(suite_runner8, CK_NORMAL);
  int failed_count8 = srunner_ntests_failed(suite_runner8);
  srunner_free(suite_runner8);

  if (failed_count8 != 0) {
    printf("Failed tests: %d\n", failed_count8);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  return exit_code;
}
