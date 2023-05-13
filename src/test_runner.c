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
Suite *Create_suite_s21_from_decimal_to_int();
// Suite *Create_suite_s21_from_decimal_to_float();
Suite *Create_suite_s21_from_float_to_decimal();
Suite *Create_suite_s21_from_int_to_decimal();
Suite *Create_suite_s21_mul();
Suite *Create_suite_s21_floor();
Suite *Create_suite_s21_negate();
Suite *Create_suite_s21_truncate();
Suite *Create_suite_s21_round();

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

  Suite *suite9 = Create_suite_s21_from_decimal_to_int();
  SRunner *suite_runner9 = srunner_create(suite9);
  srunner_run_all(suite_runner9, CK_NORMAL);
  int failed_count9 = srunner_ntests_failed(suite_runner9);
  srunner_free(suite_runner9);

  if (failed_count9 != 0) {
    printf("Failed tests: %d\n", failed_count9);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  // Suite *suite10 = Create_suite_s21_from_decimal_to_float();
  // SRunner *suite_runner10 = srunner_create(suite10);
  // srunner_run_all(suite_runner10, CK_NORMAL);
  // int failed_count10 = srunner_ntests_failed(suite_runner10);
  // srunner_free(suite_runner10);

  // if (failed_count10 != 0) {
  //   printf("Failed tests: %d\n", failed_count10);
  //   return EXIT_FAILURE;
  // } else {
  //   printf("Success\n");
  // }

  Suite *suite11 = Create_suite_s21_from_float_to_decimal();
  SRunner *suite_runner11 = srunner_create(suite11);
  srunner_run_all(suite_runner11, CK_NORMAL);
  int failed_count11 = srunner_ntests_failed(suite_runner11);
  srunner_free(suite_runner11);

  if (failed_count11 != 0) {
    printf("Failed tests: %d\n", failed_count11);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite12 = Create_suite_s21_from_int_to_decimal();
  SRunner *suite_runner12 = srunner_create(suite12);
  srunner_run_all(suite_runner12, CK_NORMAL);
  int failed_count12 = srunner_ntests_failed(suite_runner12);
  srunner_free(suite_runner12);

  if (failed_count12 != 0) {
    printf("Failed tests: %d\n", failed_count12);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite13 = Create_suite_s21_from_int_to_decimal();
  SRunner *suite_runner13 = srunner_create(suite13);
  srunner_run_all(suite_runner13, CK_NORMAL);
  int failed_count13 = srunner_ntests_failed(suite_runner13);
  srunner_free(suite_runner13);

  if (failed_count13 != 0) {
    printf("Failed tests: %d\n", failed_count13);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite14 = Create_suite_s21_floor();
  SRunner *suite_runner14 = srunner_create(suite14);
  srunner_run_all(suite_runner14, CK_NORMAL);
  int failed_count14 = srunner_ntests_failed(suite_runner14);
  srunner_free(suite_runner14);

  if (failed_count14 != 0) {
    printf("Failed tests: %d\n", failed_count14);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite15 = Create_suite_s21_negate();
  SRunner *suite_runner15 = srunner_create(suite15);
  srunner_run_all(suite_runner15, CK_NORMAL);
  int failed_count15 = srunner_ntests_failed(suite_runner15);
  srunner_free(suite_runner15);

  if (failed_count15 != 0) {
    printf("Failed tests: %d\n", failed_count15);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite16 = Create_suite_s21_truncate();
  SRunner *suite_runner16 = srunner_create(suite16);
  srunner_run_all(suite_runner16, CK_NORMAL);
  int failed_count16 = srunner_ntests_failed(suite_runner16);
  srunner_free(suite_runner16);

  if (failed_count16 != 0) {
    printf("Failed tests: %d\n", failed_count16);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  Suite *suite17 = Create_suite_s21_round();
  SRunner *suite_runner17 = srunner_create(suite17);
  srunner_run_all(suite_runner17, CK_NORMAL);
  int failed_count17 = srunner_ntests_failed(suite_runner17);
  srunner_free(suite_runner17);

  if (failed_count17 != 0) {
    printf("Failed tests: %d\n", failed_count17);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  return exit_code;
}
