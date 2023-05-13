#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

static s21_decimal num1[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
    {{0x00000000, 0x00000000, 0x00000000, 0x00030000}},
    {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}},
    {{0x00000001, 0x00000000, 0x00000000, 0x000F0000}},
    {{0xC4D5AAEC, 0x00000025, 0x00000000, 0x00060000}},
    {{0x74489B46, 0x11C6B4C4, 0x00000000, 0x00050000}},
    {{0xA4C194B9, 0xDF05E1A3, 0x000786BE, 0x00050000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},
    {{0x0098967F, 0x00000000, 0x00000000, 0x00070000}},
    {{0x0000036C, 0x00000000, 0x00000000, 0x00110000}},
};

static s21_decimal num2[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
    {{0x00000000, 0x00000000, 0x00000000, 0x80050000}},
    {{0x00000002, 0x00000000, 0x00000000, 0x00000000}},
    {{0x540BE400, 0x00000002, 0x00000000, 0x00000000}},
    {{0x0000033B, 0x00000000, 0x00000000, 0x00040000}},
    {{0x0005C187, 0x00000000, 0x00000000, 0x000F0000}},
    {{0x000003DD, 0x00000000, 0x00000000, 0x00000000}},
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},
    {{0x00000005, 0x00000000, 0x00000000, 0x00010000}},
    {{0x0D01C1BA, 0x00000000, 0x00000000, 0x00010000}},
    {{0x3E41371C, 0x01632A10, 0x00000000, 0x80040000}},
};

static s21_decimal result[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
    {{0x00000000, 0x00000000, 0x00000000, 0x80080000}},
    {{0x0000001E, 0x00000000, 0x00000000, 0x00010000}},
    {{0x540BE400, 0x00000002, 0x00000000, 0x000F0000}},
    {{0xDE3F2864, 0x00007A02, 0x00000000, 0x000A0000}},
    {{0x17B7A7EA, 0xB5E7B4AF, 0x00006651, 0x00140000}},
    {{0x7FDB8EB5, 0x99B8B533, 0x1D138F63, 0x00050000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},
    {{0x00000000, 0x00000000, 0x80000000, 0x00000000}},
    {{0x81D21746, 0x0007C0B0, 0x00000000, 0x00080000}},
    {{0x072893D0, 0xBF53EF95, 0x00000004, 0x80150000}},
};

START_TEST(test) {
  for (size_t i = 0; i < sizeof(num1) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp;
    int ret = s21_mul(num1[i], num2[i], &tmp);
    for (int j = 0; j < 4; j++)
      ck_assert_int_eq(tmp.bits[j], result[i].bits[j]);
    ck_assert_int_eq(ret, 0);
  }
}
END_TEST

Suite *Create_suite_s21_mul() {
  Suite *suite = suite_create("s21_decimal_mul tests");
  TCase *tcase_core = tcase_create("Core");
  tcase_add_test(tcase_core, test);
  suite_add_tcase(suite, tcase_core);
  return suite;
}