#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

void multiply_by_power_of_10(s21_decimal *decimal) { //
  // Multiply the decimal by 10
  unsigned int carry = 0;
  for (int i = 2; i != -1; i--) {
    unsigned long long product =
        ((unsigned long long)decimal->bits[i]) * 10 + carry;
    decimal->bits[i] = (unsigned int)(product & 0xFFFFFFFF);
    carry = (unsigned int)(product >> 32);
    // carry = ~carry + 1;
  }
  // Handle the highest 32 bits separately to prevent overflow
  unsigned long long product =
      ((unsigned long long)decimal->bits[3] & 0x7FFFFFFF) * 10 + carry;
  decimal->bits[3] =
      ((decimal->bits[3] & 0x80000000) | (unsigned int)(product & 0xFFFFFFFF));
  carry = (unsigned int)(product >> 32);
}

void __turn_info_into_decimal__(int scale, int sign, s21_decimal *dst) { //
  // Set unused bits to zero
  dst->bits[3] &= 0xFF000000;
  // Set exponent bits
  dst->bits[3] |= (scale << 16) & 0x00FF0000;
  // Set sign bit
  dst->bits[3] |= (sign << 31) & 0x80000000;
}

unsigned long long add_unsigned(unsigned int x, unsigned int y) {
  unsigned int carry, sum;
  unsigned long long result;
  carry = 0;
  result = 0;
  for (int i = 0; i < 32; i++) {
    sum = (x >> i) & 1U;
    sum += (y >> i) & 1U;
    sum += carry;
    carry = sum >> 1;
    result |= (unsigned long long)(sum & 1U) << i;
  }
  result |= (unsigned long long)carry << 32;
  return result;
}

void longIntoInts(unsigned long long result, unsigned int *a,
                  unsigned int *overflow) {
  // Take last 32 bits of number
  *a = (unsigned int)result;
  // Take first 32 bits of number
  *overflow = (unsigned int)(result >> 32);
}

int normalize_decimal(s21_decimal *a, s21_decimal *b) {

  int result_scale = 0;
  int a_scale = (a->bits[3] >> 16) & 0xFF;
  int b_scale = (b->bits[3] >> 16) & 0xFF;
  int scale_diff = a_scale - b_scale;
  scale_diff = scale_diff < 0 ? scale_diff * -1 : scale_diff;
  while (scale_diff) {
    multiply_by_power_of_10(a_scale < b_scale ? a : b);
    scale_diff--;
  }

  return a_scale > b_scale ? a_scale : b_scale;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }

  int scale = normalize_decimal(&value_1, &value_2);

  unsigned int carry = 0;
  for (int i = 2; i != -1; i--) {
    unsigned long long num = add_unsigned(value_1.bits[i], value_2.bits[i]);
    unsigned int result_value = 0;
    result->bits[i] += carry;
    longIntoInts(num, &result_value, &carry);
    result->bits[i] += result_value;
  }

  result->bits[3] |= (scale << 16) & 0x00FF0000;
}

void s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }

  int scale = normalize_decimal(&value_1, &value_2);

  unsigned int borrow = 0;
  for (int i = 2; i >= 0; i--) {
    unsigned int ai = value_1.bits[i];
    unsigned int bi = value_2.bits[i];
    result->bits[i] = ai - bi - borrow;
    borrow = (ai < bi + borrow);
  }

  result->bits[3] |= (scale << 16) & 0x00FF0000;
}

int bitLength(unsigned long long num) {
  if (num == 0) {
    return 0;
  }
  int count = 0;
  while (num != 0) {
    if (num & 1) { // check if the least significant bit is 1
      break;
    }
    count++;
    num >>= 1; // right shift by 1 bit to remove the least significant bit
  }
  return count;
}

void long_multiply(unsigned int a, unsigned int b, unsigned long long *result) {
  unsigned long long a_tmp = a;
  unsigned long long b_tmp = b;
  for (int i = 0; i < 32; i++) {
    if ((a_tmp >> i) & 1) {
      *result += b_tmp << i;
    }
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }

  unsigned int carry = 0;
  unsigned int overflow = 0;
  for (int i = 2; i >= 0; i--) {
    result->bits[i] += overflow;
    overflow = 0;
    for (int j = 2; j >= 0; j--) {
      unsigned long long borrow = 0;
      long_multiply(value_1.bits[i], value_2.bits[j], &borrow);
      unsigned int a = 0;
      longIntoInts(borrow, &a, &carry);
      overflow += carry;
      result->bits[i] += a;
    }
  }

  int a_scale = (value_1.bits[3] >> 16) & 0xFF;
  int b_scale = (value_2.bits[3] >> 16) & 0xFF;
  __turn_info_into_decimal__(a_scale + b_scale, 0, result);
}

int main() {

  s21_decimal p1 = {0, 4294967295, 4294967295, 0};
  s21_decimal p2 = {0, 0, 2, 0};

  s21_decimal result = {0, 0, 0, 0};
  s21_div(p1, p2, &result);

  printf("%u\n", result.bits[0]);
  printf("%u\n", result.bits[1]);
  printf("%u\n", result.bits[2]);
  printf("%u\n", result.bits[3]);

  return 0;
}