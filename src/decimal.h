#include <stdio.h>
#include <stdbool.h>
#include <math.h>
typedef struct {
  int bits[4];
} s21_decimal;
void perform_decimal_into_binary(int number, int bits, _Bool* __binary__);
void __converter__(int number, _Bool* __binary__);
int __predict_len__(int number);