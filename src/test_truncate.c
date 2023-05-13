#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

static s21_decimal arr[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},

    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
    {{0x00000000, 0x00000000, 0x00000000, 0x00020000}},
    {{0x00000000, 0x00000000, 0x00000000, 0x80020000}},

    {{0x00000467, 0x0000007A, 0x00003215, 0x00010000}},
    {{0x00000467, 0x0000007A, 0x00205FF0, 0x80020000}},
    {{0x000F4240, 0x00002710, 0x00002710, 0x80050000}},

    {{0x00000019, 0x00000000, 0x00000000, 0x80010000}},
    {{0x1FC40A5F, 0xA98FD4BE, 0x00000000, 0x00020000}},
    {{0x1FC40A5D, 0xA98FD4BE, 0x00000000, 0x00020000}},
    {{0x1FC40A5F, 0xA98FD4BE, 0x00000000, 0x80020000}},
    {{0x1FC40A5D, 0xA98FD4BE, 0x00000000, 0x80020000}},

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
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},

    {{0xCCCCCD3D, 0x199999A5, 0x00000502, 0x00000000}},
    {{0xBD70A3E2, 0x1EB851EC, 0x000052E1, 0x80000000}},
    {{0xB333333D, 0x19999999, 0x00000000, 0x80000000}},

    {{0x00000002, 0x00000000, 0x00000000, 0x80000000}},
    {{0x24285C43, 0x01B2140C, 0x00000000, 0x00000000}},
    {{0x24285C43, 0x01B2140C, 0x00000000, 0x00000000}},
    {{0x24285C43, 0x01B2140C, 0x00000000, 0x80000000}},
    {{0x24285C43, 0x01B2140C, 0x00000000, 0x80000000}},

    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},

    {{0x00000007, 0x00000000, 0x00000000, 0x00000000}},
    {{0xAC471B47, 0x0000A7C5, 0x00000000, 0x80000000}},
    {{0x00000007, 0x00000000, 0x00000000, 0x80000000}},
};

START_TEST(test) {
  for (size_t i = 0; i < sizeof(arr) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp;
    int ret = s21_truncate(arr[i], &tmp);
    for (int j = 0; j < 4; j++)
      ck_assert_int_eq(tmp.bits[j], result[i].bits[j]);
    ck_assert_int_eq(ret, 0);
  }
}
END_TEST

START_TEST(error_test) {
  s21_decimal a = {{10, 13, 827, 30 << 16}};
  s21_decimal b;
  int ret = s21_truncate(a, &b);
  ck_assert_int_eq(ret, 1);
  for (int j = 0; j < 4; j++)
    ck_assert_int_eq(b.bits[j], 0);
}
END_TEST

Suite *Create_suite_s21_truncate(void) {
  Suite *suite = suite_create("s21_truncate");
  TCase *tcase_core = tcase_create("s21_truncate");
  if (suite != NULL && tcase_core != NULL) {
    tcase_add_test(tcase_core, test);
    tcase_add_test(tcase_core, error_test);
    suite_add_tcase(suite, tcase_core);
  }
  return (suite);
}
