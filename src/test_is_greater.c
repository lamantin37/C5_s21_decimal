#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

static s21_decimal lhs[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x800A0000}},
    {{0x00000001, 0x00000000, 0x00000000, 0x00000000}},
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},
    {{0x0000000F, 0x00000014, 0x0000072E, 0x800A0000}},
    {{0x0000000F, 0x00000014, 0x0000072E, 0x00020000}},
    {{0x99362457, 0x0007AE79, 0x00000000, 0x00080000}},
    {{0x2113B8B0, 0x00000917, 0x00000000, 0x80070000}},
    {{0x00000085, 0x00000054, 0x00000013, 0x00010000}},
    {{0x000011EB, 0x00000000, 0x00000000, 0x80010000}},
    {{0x07A5B45C, 0x00000000, 0x00000000, 0x00050000}},
    {{0x39608B53, 0x0000001C, 0x00000000, 0x00050000}},
    {{0x39608B53, 0x0000001C, 0x00000000, 0x00050000}},
    {{0x000046CB, 0x00000000, 0x00000000, 0x80030000}},
    {{0x000046CA, 0x00000000, 0x00000000, 0x80030000}},
    {{0x000046CC, 0x00000000, 0x00000000, 0x80030000}},
    {{0x000046CB, 0x00000000, 0x00000000, 0x00030000}},
};

static s21_decimal rhs[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00030000}},
    {{0x00000001, 0x00000000, 0x00000000, 0x00000000}},
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},
    {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},
    {{0x0000000F, 0x00000013, 0x0000072E, 0x800A0000}},
    {{0x0000000F, 0x00000014, 0x0000072F, 0x00020000}},
    {{0x99362457, 0x0007AE79, 0x00000000, 0x00080000}},
    {{0x2113B8B0, 0x00000917, 0x00000000, 0x80070000}},
    {{0x00003458, 0x00000054, 0x00000013, 0x00030000}},
    {{0x000011EB, 0x00000000, 0x00000000, 0x80020000}},
    {{0x001393BA, 0x00000000, 0x00000000, 0x00030000}},
    {{0x39608B53, 0x0000001C, 0x00000000, 0x00050000}},
    {{0x39608B52, 0x0000001C, 0x00000000, 0x00050000}},
    {{0x000042E3, 0x00000000, 0x00000000, 0x80030000}},
    {{0x000042E3, 0x00000000, 0x00000000, 0x80030000}},
    {{0x000042E3, 0x00000000, 0x00000000, 0x80030000}},
    {{0x000042E3, 0x00000000, 0x00000000, 0x00030000}},
};

static int result[] = {
    0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
};

START_TEST(test) {
  for (size_t i = 0; i < sizeof(lhs) / sizeof(s21_decimal); ++i) {
    int ret = s21_is_greater(lhs[i], rhs[i]);
    ck_assert_int_eq(ret, result[i]);
  }
}
END_TEST

Suite *Create_suite_s21_is_greater() {
  Suite *suite = suite_create("s21_decimal_is_greater tests");
  TCase *tcase_core = tcase_create("Core");
  tcase_add_test(tcase_core, test);
  suite_add_tcase(suite, tcase_core);
  return suite;
}