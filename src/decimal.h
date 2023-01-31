#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MINIMUM_INT -2147483648
#define MAXIMUM_INT 2147483647

typedef struct {
  int bits[4];
} s21_decimal;
void perform_decimal_into_binary(int number, int bits, _Bool* __binary__);
void __converter__(int number, _Bool* __binary__);
int __predict_len__(int number);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);