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

void multiply_by_power_of_10(s21_decimal *decimal, int use);
unsigned int divide_by_10(s21_decimal *decimal);
void __turn_info_into_decimal__(int scale, int sign, s21_decimal *dst);
int normalize_decimal(s21_decimal *a, s21_decimal *b);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int _s21_add_(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int use);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int _s21_sub_(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
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
int check_zero_decimal(s21_decimal value);

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
