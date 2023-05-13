#include <check.h>
#include <stdio.h>

#include "decimal.h"

static s21_decimal num1[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 0: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x00030000}},  // i= 1: 0.000
    {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}},  // i= 2: 1.5
    {{0x00000001, 0x00000000, 0x00000000,
      0x000F0000}},  // i= 3: 0.000000000000001
    {{0xC4D5AAEC, 0x00000025, 0x00000000, 0x00060000}},  // i= 4: 162216.127212
    {{0x74489B46, 0x11C6B4C4, 0x00000000,
      0x00050000}},  // i= 5: 12809098998909.89894
    {{0xA4C194B9, 0xDF05E1A3, 0x000786BE,
      0x00050000}},  // i= 6: 90987987978798797979.37337
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  // i= 7: -79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // i= 8: 79228162514264337593543950335
    {{0x0098967F, 0x00000000, 0x00000000, 0x00070000}},  // i=13: 0.9999999
    {{0x0000036C, 0x00000000, 0x00000000,
      0x00110000}},  // i=14: 0.00000000000000876
};

static s21_decimal num2[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 0: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x80050000}},  // i= 1: 0.00000
    {{0x00000002, 0x00000000, 0x00000000, 0x00000000}},  // i= 2: 2
    {{0x540BE400, 0x00000002, 0x00000000, 0x00000000}},  // i= 3: 10000000000
    {{0x0000033B, 0x00000000, 0x00000000, 0x00040000}},  // i= 4: 0.0827
    {{0x0005C187, 0x00000000, 0x00000000,
      0x000F0000}},  // i= 5: 0.000000000377223
    {{0x000003DD, 0x00000000, 0x00000000, 0x00000000}},  // i= 6: 989
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},  // i= 7: -1
    {{0x00000005, 0x00000000, 0x00000000, 0x00010000}},  // i= 8: 0.5
    {{0x0D01C1BA, 0x00000000, 0x00000000, 0x00010000}},  // i=13: 21821893.8
    {{0x3E41371C, 0x01632A10, 0x00000000,
      0x80040000}},  // i=14: -9996986598458.7548
};

static s21_decimal result[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 0: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x80080000}},  // i= 1: 0.00000000
    {{0x0000001E, 0x00000000, 0x00000000, 0x00010000}},  // i= 2: 3.0
    {{0x540BE400, 0x00000002, 0x00000000,
      0x000F0000}},  // i= 3: 0.000010000000000
    {{0xDE3F2864, 0x00007A02, 0x00000000,
      0x000A0000}},  // i= 4: 13415.2737204324
    {{0x17B7A7EA, 0xB5E7B4AF, 0x00006651,
      0x00140000}},  // i= 5: 4831.88675166578880784362
    {{0x7FDB8EB5, 0x99B8B533, 0x1D138F63,
      0x00050000}},  // i= 6: 89987120111032011201600.26293
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // i= 7: 79228162514264337593543950335
    {{0x00000000, 0x00000000, 0x80000000,
      0x00000000}},  // i= 8: 39614081257132168796771975168
    {{0x81D21746, 0x0007C0B0, 0x00000000,
      0x00080000}},  // i=13: 21821891.61781062
    {{0x072893D0, 0xBF53EF95, 0x00000004,
      0x80150000}},  // i=14: -0.087573602602498692048
};

START_TEST(test) {
  for (size_t i = 0; i < sizeof(num1) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp;
    int ret = s21_mul(num1[i], num2[i], &tmp);
    ck_assert_int_eq(tmp.bits[0], result[i].bits[0]);
    ck_assert_int_eq(tmp.bits[1], result[i].bits[1]);
    ck_assert_int_eq(tmp.bits[2], result[i].bits[2]);
    ck_assert_int_eq(tmp.bits[3], result[i].bits[3]);
    ck_assert_int_eq(ret, 0);
    printf("%ld\n", i);
  }
}
END_TEST

Suite *Create_suite() {
  Suite *suite = suite_create("s21_decimal");
  TCase *tcase_core = tcase_create("Core");
  tcase_add_test(tcase_core, test);
  suite_add_tcase(suite, tcase_core);
  return suite;
}
int main(void) {
  Suite *suite = Create_suite();
  SRunner *suite_runner = srunner_create(suite);
  srunner_run_all(suite_runner, CK_NORMAL);
  int failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  if (failed_count != 0) {
    printf("Failed tests: %d\n", failed_count);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }
  return EXIT_SUCCESS;
}
