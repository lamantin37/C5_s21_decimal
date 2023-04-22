#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXIMUM_UNSIGNED_INT 4294967295
#define MINIMUM_INT -2147483648
#define MAXIMUM_INT 2147483647

typedef struct {
  unsigned int bits[4];
} s21_decimal;

void multiply_by_power_of_10(s21_decimal *decimal);
void __turn_info_into_decimal__(int scale, int sign, s21_decimal *dst);
unsigned long long add_unsigned(unsigned int x, unsigned int y);
void longIntoInts(unsigned long long result, unsigned int *a,
                  unsigned int *overflow);
int normalize_decimal(s21_decimal *a, s21_decimal *b);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int _s21_add_(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int _s21_sub_(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int bitLength(unsigned long long num);
void long_multiply(unsigned int a, unsigned int b, unsigned long long *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/////////////////////////////////////////////////////////////////

int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);

////////////////////////////////////////////////////////////////

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void division_of_the_remainder(s21_decimal value_1, s21_decimal quotient,
                               s21_decimal value_2, s21_decimal *result);
int check_zero_decimal(s21_decimal value);
void convert_binary_into_decimal(_Bool *value, s21_decimal *result);
void perform_decimal_into_binary(int number, int bits, _Bool *__binary__);
void __converter__(int number, _Bool *__binary__);
int __predict_len__(int number);
int countDigits(unsigned int num);

/////////////////////////////////////////////////////////////////

int poly_div(_Bool *binary1, _Bool *binary2, _Bool *result, int mod_flag);
void __div_decimal_convert__(s21_decimal value_1, int *decimal);
void ____div_decimal_pow____(int *__int1__, int power, int *__result__);
void ______div_decimal_add______(int *__int1__, int *__int2__, int *__result__);
int __s21_is_equal__(_Bool *__binary1__, _Bool *__binary2__);
int take_element(_Bool *binary1, _Bool *binary2, _Bool *result);
int __s21_is_greater_or_equal__(_Bool *__binary1__, _Bool *__binary2__);
void __s21_sub__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/////////////////////////////////////////////////////////////////

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

/////////////////////////////////////////////////////////////////

int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
