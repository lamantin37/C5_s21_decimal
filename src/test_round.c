#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

static s21_decimal arr[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
    {{0x00000000, 0x00000000, 0x00000000, 0x00020000}},
    {{0x0000000E, 0x00000000, 0x00000000, 0x00000000}},
    {{0x0000000F, 0x00000000, 0x00000000, 0x80000000}},
    {{0x0000000B, 0x00000000, 0x00000000, 0x80010000}},
    {{0x00000095, 0x00000000, 0x00000000, 0x80020000}},
    {{0xC2AC6A00, 0x00000021, 0x00000000, 0x000A0000}},
    {{0x1FC40A5F, 0xA98FD4BE, 0x00000000, 0x00020000}},
    {{0x1FC40A5D, 0xA98FD4BE, 0x00000000, 0x00020000}},
    {{0x075BCD15, 0x00000000, 0x00000000, 0x001B0000}},
    {{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00060000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00050000}},
    {{0x075BCD15, 0x00000000, 0x00000000, 0x801B0000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x001C0000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x80050000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x801C0000}},
};

static s21_decimal result[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
    {{0x0000000E, 0x00000000, 0x00000000, 0x00000000}},
    {{0x0000000F, 0x00000000, 0x00000000, 0x80000000}},
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},
    {{0x0000000F, 0x00000000, 0x00000000, 0x00000000}},
    {{0x24285C44, 0x01B2140C, 0x00000000, 0x00000000}},
    {{0x24285C43, 0x01B2140C, 0x00000000, 0x00000000}},
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
    {{0x000010C7, 0x00000000, 0x00000000, 0x00000000}},
    {{0xAC471B48, 0x0000A7C5, 0x00000000, 0x00000000}},
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},
    {{0x00000008, 0x00000000, 0x00000000, 0x00000000}},
    {{0xAC471B48, 0x0000A7C5, 0x00000000, 0x80000000}},
    {{0x00000008, 0x00000000, 0x00000000, 0x80000000}},
};

START_TEST(test) {
  for (size_t i = 0; i < sizeof(arr) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp;
    int ret = s21_round(arr[i], &tmp);
    ck_assert_int_eq(tmp.bits[0], result[i].bits[0]);
    ck_assert_int_eq(tmp.bits[1], result[i].bits[1]);
    ck_assert_int_eq(tmp.bits[2], result[i].bits[2]);
    ck_assert_int_eq(tmp.bits[3], result[i].bits[3]);
    ck_assert_int_eq(ret, 0);
  }
}
END_TEST

START_TEST(error_test) {
  s21_decimal a = {{10, 13, 827, 30 << 16}};
  s21_decimal b;
  int ret = s21_round(a, &b);
  ck_assert_int_eq(ret, 1);
  ck_assert_int_eq(b.bits[0], 0);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq(b.bits[3], 0);
}
END_TEST

Suite *Create_suite_s21_round() {
  Suite *suite = suite_create("s21_round");
  TCase *tcase_core = tcase_create("s21_round");
  if (suite != NULL && tcase_core != NULL) {
    tcase_add_test(tcase_core, test);
    tcase_add_test(tcase_core, error_test);
    suite_add_tcase(suite, tcase_core);
  }
  return (suite);
}