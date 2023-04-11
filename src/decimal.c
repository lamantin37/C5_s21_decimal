#include "decimal.h"

int main() {
  s21_decimal value = {174, 0, 0, 0};
  __turn_info_into_decimal__(4, 0, &value);
  s21_decimal result;

  s21_truncate(value, &result);
  printf("%u\n", result.bits[0]);
  printf("%u\n", result.bits[1]);
  printf("%u\n", result.bits[2]);
  printf("%u\n", result.bits[3]);

  return 0;
}

//////////////////////////////////////////////////////////

void multiply_by_power_of_10(s21_decimal *decimal) {
  // Multiply the decimal by 10
  unsigned int carry = 0;
  for (int i = 0; i != 3; i++) {
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

void __turn_info_into_decimal__(int scale, int sign, s21_decimal *dst) {
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
                  unsigned int *overflow) { //
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
  result->bits[3] |= (scale << 16) & 0x00FF0000;

  unsigned int carry = 0;
  unsigned int carry_case = 0;
  for (int i = 0; i != 3; i++) {
    unsigned long long num = add_unsigned(value_1.bits[i], value_2.bits[i]);
    unsigned int result_value = 0;

    unsigned long long tmp = (unsigned long long)value_1.bits[i] +
                             (unsigned long long)value_2.bits[i] +
                             (unsigned long long)carry;
    carry_case = (tmp > MAXIMUM_UNSIGNED_INT) && (carry & 1) ? 1 : 0;

    result->bits[i] += carry;
    longIntoInts(num, &result_value, &carry);
    carry += carry_case;
    result->bits[i] += result_value;
  }

  if (carry != 0) {
    for (int i = 0; i != 4; i++) {
      result->bits[i] = 0;
    }
    return 1;
  }

  return 0;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Initialize the result to zero
  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }

  int scale = normalize_decimal(&value_1, &value_2);
  result->bits[3] |= (scale << 16) & 0x00FF0000;

  // Subtract the integer parts
  unsigned int borrow = 0;
  for (int i = 0; i != 3; i++) {
    unsigned int diff = value_1.bits[i] - value_2.bits[i] - borrow;
    if (value_1.bits[i] < value_2.bits[i] ||
        (value_1.bits[i] == value_2.bits[i] && borrow != 0)) {
      borrow = 1;
    } else {
      borrow = 0;
    }
    result->bits[i] = diff;
  }

  return 0; // success
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

///////////////////////////////////////////////////////////////

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Check for zero or infinity
  if (((value_1.bits[3] & 0x7FFFFFFF) == 0 && value_1.bits[0] == 0 &&
       value_1.bits[1] == 0 && value_1.bits[2] == 0) ||
      ((value_2.bits[3] & 0x7FFFFFFF) == 0 && value_2.bits[0] == 0 &&
       value_2.bits[1] == 0 && value_2.bits[2] == 0)) {
    result->bits[0] = 0;
    result->bits[1] = 0;
    result->bits[2] = 0;
    result->bits[3] =
        (value_1.bits[3] & 0x80000000) ^ (value_2.bits[3] & 0x80000000);
    return 0;
  }
  if (((value_1.bits[3] & 0x7FFFFFFF) == 0x7F800000 && value_1.bits[0] == 0 &&
       value_1.bits[1] == 0 && value_1.bits[2] == 0) ||
      ((value_2.bits[3] & 0x7FFFFFFF) == 0x7F800000 && value_2.bits[0] == 0 &&
       value_2.bits[1] == 0 && value_2.bits[2] == 0)) {
    result->bits[0] = 0;
    result->bits[1] = 0;
    result->bits[2] = 0;
    result->bits[3] =
        0x7F800000 | ((value_1.bits[3] ^ value_2.bits[3]) & 0x80000000);
    return 0;
  }
  // Calculate the sign of the result
  int sign = ((value_1.bits[3] >> 31) & 1) ^ ((value_2.bits[3] >> 31) & 1);
  result->bits[3] = sign << 31;

  // Calculate the exponent of the result
  int exponent = ((value_1.bits[3] >> 16) & 0xFF) +
                 ((value_2.bits[3] >> 16) & 0xFF) - 0x3F800000;
  unsigned int low, mid, high;

  // Multiply the mantissas
  unsigned long long int product_low = (unsigned long long int)value_1.bits[0] *
                                       (unsigned long long int)value_2.bits[0];
  unsigned long long int product_mid =
      (unsigned long long int)value_1.bits[0] *
          (unsigned long long int)value_2.bits[1] +
      (unsigned long long int)value_1.bits[1] *
          (unsigned long long int)value_2.bits[0];
  unsigned long long int product_high =
      (unsigned long long int)value_1.bits[2] *
      (unsigned long long int)value_2.bits[2];

  // Add the carry from the low part to the middle part
  product_mid += (product_low >> 32);

  // Add the carry from the middle part to the high part
  product_high += (product_mid >> 32);

  // Check for overflow
  if (product_high & 0xFFFFFFFF00000000ULL) {
    exponent++;
    product_high >>= 32;
    product_mid >>= 32;
  }

  // Store the result in the output parameter
  result->bits[0] = (unsigned int)product_low;
  result->bits[1] = (unsigned int)product_mid;
  result->bits[2] = (unsigned int)product_high;

  // Set the exponent and sign in the output parameter
  result->bits[3] |= ((unsigned int)exponent << 16);

  return 0;
}

///////////////////////////////////////////////////////////////

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Check if both values are equal
  if (value_1.bits[0] == value_2.bits[0] &&
      value_1.bits[1] == value_2.bits[1] &&
      value_1.bits[2] == value_2.bits[2]) {
    result->bits[0] = 1;
    result->bits[1] = 0;
    result->bits[2] = 0;
    result->bits[3] = 0;
    return 0;
  }

  // Check if divisor is zero
  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
    return 2;
  }

  // Determine the scale factor and sign of the result
  int scale_factor = (value_1.bits[3] & 0x00FF0000) >> 16;
  scale_factor -= (value_2.bits[3] & 0x00FF0000) >> 16;
  int sign = (value_1.bits[3] ^ value_2.bits[3]) & 0x80000000;

  // Initialize remainder to zero
  unsigned int remainder = 0;

  // Perform the division
  for (int i = 2; i >= 0; i--) {
    int shift = 0;
    if (value_2.bits[i] == 0) {
      while (value_2.bits[i + shift] == 0) {
        shift--;
      }
    }
    unsigned long long quotient = (unsigned long long)remainder;
    quotient <<= 32;
    quotient += value_1.bits[i];
    result->bits[i] = (unsigned int)(quotient / value_2.bits[i + shift]);
    remainder = (unsigned int)(quotient - (unsigned long long)result->bits[i] *
                                              value_2.bits[i + shift]);
  }

  // Set the scale factor and sign of the result
  result->bits[3] = (scale_factor << 16) | sign;

  return 0;
}

void division_of_the_remainder(s21_decimal integer_part, s21_decimal quotient,
                               s21_decimal divisor, s21_decimal *result) {

  s21_decimal temp = {0, 0, 0, 0};
  s21_mul(integer_part, divisor, &integer_part);
  s21_sub(quotient, integer_part, &temp);

  printf("bits[0] = %u\n", temp.bits[0]);
  printf("bits[1] = %u\n", temp.bits[1]);
  printf("bits[2] = %u\n", temp.bits[2]);
  printf("bits[3] = %u\n\n", temp.bits[3]);
}

///////////////////////////////////////////////////////////////

int s21_is_equal(s21_decimal a, s21_decimal b) {
  // Compare the sign bit first
  int sign_a = (a.bits[3] >> 31) & 1;
  int sign_b = (b.bits[3] >> 31) & 1;

  if (sign_a != sign_b) {
    return 0;
  }

  // Compare the scale factors
  int scale_a = (a.bits[3] >> 16) & 0xFF;
  int scale_b = (b.bits[3] >> 16) & 0xFF;

  if (scale_a != scale_b) {
    return 0;
  }

  // Compare the integer parts
  for (int i = 2; i >= 0; i--) {
    if (a.bits[i] != b.bits[i]) {
      return 0;
    }
  }

  return 1; // The two values are equal
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  // Check signs
  int aIsNegative = (a.bits[3] & 0x80000000) != 0;
  int bIsNegative = (b.bits[3] & 0x80000000) != 0;

  if (aIsNegative != bIsNegative) {
    return aIsNegative;
  }

  // Check scales
  int aScale = (a.bits[3] >> 16) & 0xFF;
  int bScale = (b.bits[3] >> 16) & 0xFF;
  if (aScale != bScale) {
    return (aIsNegative ? aScale < bScale : aScale > bScale);
  }

  // Check integer parts
  for (int i = 2; i >= 0; i--) {
    if (a.bits[i] != b.bits[i]) {
      return (aIsNegative ? a.bits[i] < b.bits[i] : a.bits[i] > b.bits[i]);
    }
  }

  // Numbers are equal
  return 1;
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  return (!(s21_is_equal(a, b) || s21_is_greater(a, b)));
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return (s21_is_less(a, b) || s21_is_equal(a, b));
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return (s21_is_greater(a, b) || s21_is_equal(a, b));
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return (!s21_is_equal(a, b));
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  result->bits[0] = value.bits[0];
  result->bits[1] = value.bits[1];
  result->bits[2] = value.bits[2];
  result->bits[3] = (value.bits[3] & 0x7F000000) | value.bits[3] ^ 0x80000000;
  return 0;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  for (int i = 0; i != 4; i++) {
    dst->bits[i] = 0;
  }
  __turn_info_into_decimal__(0, src < 0 ? 1 : 0, dst);
  dst->bits[0] = src < 0 ? -src : src;
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {

  int src_scale = (src.bits[3] >> 16) & 0xFF;
  int sign_src = (src.bits[3] >> 31) & 1;
  unsigned int high_bits = src.bits[2];
  unsigned long long low_middle_bits = 0;

  if (src_scale <= 10) {
    low_middle_bits = ((unsigned long long)src.bits[1]) << 32 | src.bits[0];
    for (int i = 0; i != src_scale; i++) {
      low_middle_bits /= 10;
    }
    if (low_middle_bits > MAXIMUM_INT || src.bits[2] != 0) {
      return 1;
    }
    *dst = (unsigned int)low_middle_bits;
  } else if (src_scale <= 20) {
    low_middle_bits = ((unsigned long long)src.bits[2]) << 32 | src.bits[1];
    for (int i = 0; i != src_scale - 10; i++) {
      low_middle_bits /= 10;
    }
    if (low_middle_bits > MAXIMUM_INT) {
      return 1;
    }
    *dst = (unsigned int)low_middle_bits;
  } else {
    *dst = src.bits[2];
    for (int i = 0; i != src_scale - 20; i++) {
      *dst /= 10;
    }
  }

  *dst *= sign_src ? -1 : 1;
  return 0;
}

int reverse_number(int num) {
  int reversed_num = 0;
  while (num > 0) {
    reversed_num = reversed_num * 10 + num % 10;
    num /= 10;
  }
  return reversed_num;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  char buf[40] = "\0";
  sprintf(buf, "%.7g", src);

  int scale = 0;
  int sign = 0;

  unsigned int decimal = 0;
  int factor = 0;
  for (char *p = buf; *p != '\0'; p++) {
    if (*p != '.' && *p != '-') {
      decimal += (*p - 48) * pow(10, factor);
      factor++;
    } else {
      if (*p == '.') {
        scale = factor;
      } else if (*p == '-') {
        sign = 1;
      }
    }
  }

  scale = strlen(buf) - 1 - sign - scale;
  __turn_info_into_decimal__(scale, sign, dst);

  dst->bits[0] = reverse_number(decimal);

  return 0;
}

int countDigits(int num) {
  int count = 0;

  if (num < 0) {
    num = -num;
  }

  while (num != 0) {
    count++;
    num /= 10;
  }

  return count;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {

  int decimal_float_part = 0;
  if (s21_from_decimal_to_int(src, &decimal_float_part) == 1) {
    return 1;
  }
  int lenght = countDigits(decimal_float_part);

  int src_scale = (src.bits[3] >> 16) & 0xFF;
  int new_scale = src_scale - (7 - lenght) > 0 ? src_scale - (7 - lenght) : 0;
  __turn_info_into_decimal__(new_scale, 0, &src);
  if (s21_from_decimal_to_int(src, &decimal_float_part) == 1) {
    return 1;
  }

  *dst = decimal_float_part;
  while (countDigits((int)*dst) != lenght) {
    *dst *= 0.1;
  }

  return 0;
}

/////////////////////////////////////////////////////////////////

int s21_truncate(s21_decimal value, s21_decimal *result) {
  unsigned int scale = (value.bits[3] >> 16) & 0x000000FF;
  int sign = value.bits[3] >> 31;
  if (scale = 0) {
    *result = value;
    return 0;
  }
  unsigned int power = 1;
  for (unsigned int i = 0; i < scale; i++) {
    power *= 10;
  }
  s21_decimal integral;
  for (int i = 2; i >= 0; i--) {
    unsigned long long temp = ((unsigned long long)value.bits[i]) * power;
    integral.bits[i] = (unsigned int)(temp >> 32);
    value.bits[i] = (unsigned int)(temp & 0xFFFFFFFF);
  }
  integral.bits[3] = (scale << 16) | sign;
  *result = integral;

  return 0;
}
