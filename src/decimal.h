#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define MINIMUM_INT -2147483648
#define MAXIMUM_INT 2147483647

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int position;
  int minus;
} s21_decimal_info;

void perform_decimal_into_binary(int number, int bits, _Bool *__binary__);
void __converter__(int number, _Bool *__binary__);
void convert_binary_into_decimal(_Bool *value, s21_decimal *result);
int __predict_len__(int number);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void __s21_add__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void __s21_sub__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void __s21_mul__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int __s21_div__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__);
int take_element(_Bool *binary1, _Bool *binary2, _Bool *result);
int poly_div(_Bool *binary1, _Bool *binary2, _Bool *result, int mod_flag);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void __take_info__(s21_decimal_info *info, s21_decimal value_1);
void __div_decimal__(s21_decimal value_1, s21_decimal *__int__, s21_decimal *__point__, int position);
void ____div_decimal_pow____(int *__int1__, int power, int *__result__);
void ______div_decimal_add______(int *__int1__, int *__int2__, int *__result__);
void ______div_decimal_sub______(int *__int1__, int *__int2__, int *__result__);
void __div_decimal_convert__(s21_decimal value_1, int *decimal);
int __div_decimal_higher_or_equal__(int *__int1__, int *__int2__);
void __div_perform_back__(int *__int1__, _Bool *__binary1__);
void __div_convert_back(int *decimal_int, int *decimal_point, s21_decimal *result, int position_result);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int __s21_is_greater_or_equal__(_Bool *__binary1__, _Bool *__binary2__);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int __s21_is_equal__(_Bool *__binary1__, _Bool *__binary2__);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
void __turn_info_into_decimal__(int position, int minus, s21_decimal *dst);