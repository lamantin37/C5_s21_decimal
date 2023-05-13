#include "decimal.h"

// int main() {
//   float a = 0.f;
//   s21_decimal res = {0x0001869F, 0x00000000, 0x00000000, 0x00040000};
//   s21_from_decimal_to_float(res, &a);

//   printf("%f\n", a);
// }

void multiply_by_power_of_10(s21_decimal *decimal) {
  unsigned int carry = 0;
  for (int i = 0; i != 3; i++) {
    unsigned long long product =
        ((unsigned long long)decimal->bits[i]) * 10 + carry;
    decimal->bits[i] = (unsigned int)(product & 0xFFFFFFFF);
    carry = (unsigned int)(product >> 32);
  }
}

void __turn_info_into_decimal__(int scale, int sign, s21_decimal *dst) {
  dst->bits[3] &= 0xFF000000;
  dst->bits[3] |= (scale << 16) & 0x00FF0000;
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
  *a = (unsigned int)result;
  *overflow = (unsigned int)(result >> 32);
}

int normalize_decimal(s21_decimal *a, s21_decimal *b) {
  int sign_a = (a->bits[3] >> 31) & 1;
  int sign_b = (b->bits[3] >> 31) & 1;
  int a_scale = (a->bits[3] >> 16) & 0xFF;
  int b_scale = (b->bits[3] >> 16) & 0xFF;
  int result_scale = a_scale > b_scale ? a_scale : b_scale;
  int scale_diff = a_scale - b_scale;
  scale_diff = scale_diff < 0 ? scale_diff * -1 : scale_diff;
  while (scale_diff) {
    if (((unsigned long long)(a_scale < b_scale ? a->bits[2] : b->bits[2]) *
         10) <= MAXIMUM_UNSIGNED_INT) {
      multiply_by_power_of_10(a_scale < b_scale ? a : b);
    } else {

      a_scale > b_scale ? (a->bits[3] = (scale_diff << 16) & 0x00FF0000)
                        : (b->bits[3] = (scale_diff << 16) & 0x00FF0000);
      s21_round(a_scale > b_scale ? *a : *b, a_scale > b_scale ? a : b);
      a_scale > b_scale ? (a_scale -= scale_diff) : (b_scale -= scale_diff);
      a_scale > b_scale ? (a->bits[3] = (a_scale << 16) & 0x00FF0000)
                        : (b->bits[3] = (b_scale << 16) & 0x00FF0000);
      result_scale -= scale_diff;
      a->bits[3] |= sign_a << 31;
      b->bits[3] |= sign_b << 31;
      break;
    }
    scale_diff--;
  }

  return result_scale;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }
  if (value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0 &&
      value_1.bits[3] >> 31 & 1) {
    int a_scale = (value_1.bits[3] >> 16) & 0xFF;
    value_1.bits[3] = 0;
    __turn_info_into_decimal__(a_scale, 0, &value_1);
  }
  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0 &&
      value_2.bits[3] >> 31 & 1) {
    int a_scale = (value_2.bits[3] >> 16) & 0xFF;
    value_2.bits[3] = 0;
    __turn_info_into_decimal__(a_scale, 0, &value_2);
  }
  int sign_a = (value_1.bits[3] >> 31) & 1;
  int sign_b = (value_2.bits[3] >> 31) & 1;
  int result_func = 0;
  if (sign_a > sign_b) {
    s21_negate(value_1, &value_1);
    if (s21_is_greater(value_1, value_2)) {
      result->bits[3] |= (1 << 31);
      result_func = _s21_sub_(value_1, value_2, result);
    } else {
      result_func = _s21_sub_(value_2, value_1, result);
    }
  } else if (sign_b > sign_a) {
    s21_negate(value_2, &value_2);
    if (s21_is_greater(value_2, value_1)) {
      result->bits[3] |= (1 << 31);
      result_func = _s21_sub_(value_2, value_1, result);
    } else {
      result_func = _s21_sub_(value_1, value_2, result);
    }
  } else if (sign_b == 1 && sign_b == 1) {
    result->bits[3] |= (1 << 31);
    result_func = _s21_add_(value_1, value_2, result);
    result_func = result_func == 1 ? 2 : 0;
  } else {
    result_func = _s21_add_(value_1, value_2, result);
  }
  return result_func;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }
  int sign_a = (value_1.bits[3] >> 31) & 1;
  int sign_b = (value_2.bits[3] >> 31) & 1;
  int result_func = 0;
  if (sign_a > sign_b) {
    result->bits[3] |= (1 << 31);
    result_func = _s21_add_(value_1, value_2, result);
    result_func = result_func == 1 ? 2 : 0;
  } else if (sign_b > sign_a) {
    result_func = _s21_add_(value_1, value_2, result);
  } else if (sign_b == 1 && sign_b == 1) {
    s21_negate(value_1, &value_1);
    s21_negate(value_2, &value_2);
    if (s21_is_greater_or_equal(value_1, value_2)) {
      result->bits[3] |= (1 << 31);
      result_func = _s21_sub_(value_1, value_2, result);
    } else {
      result_func = _s21_sub_(value_2, value_1, result);
    }
  } else {
    if (s21_is_greater_or_equal(value_1, value_2)) {
      result_func = _s21_sub_(value_1, value_2, result);
    } else {
      result->bits[3] |= (1 << 31);
      result_func = _s21_sub_(value_2, value_1, result);
    }
  }

  return result_func;
}

int _s21_add_(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
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

int _s21_sub_(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int scale = normalize_decimal(&value_1, &value_2);
  result->bits[3] |= (scale << 16) & 0x00FF0000;
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
  return 0;
}

int bitLength(unsigned long long num) {
  if (num == 0) {
    return 0;
  }
  int count = 0;
  while (num != 0) {
    if (num & 1) {
      break;
    }
    count++;
    num >>= 1;
  }
  return count;
}

///////////////////////////////////////////////////////////////

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }
  int a_scale = (value_1.bits[3] >> 16) & 0xFF;
  int b_scale = (value_2.bits[3] >> 16) & 0xFF;
  int sign_a = (value_1.bits[3] >> 31) & 1;
  int sign_b = (value_2.bits[3] >> 31) & 1;

  s21_decimal ten_exp = {10, 0, 0, 0};
  value_2.bits[3] = 0;
  while (s21_is_greater(value_2, ten_exp)) {
    s21_decimal multiplicand = value_1;
    multiplicand.bits[3] = 0;
    while (s21_is_greater_or_equal(value_2, ten_exp)) {
      multiply_by_power_of_10(&ten_exp);
      multiply_by_power_of_10(&multiplicand);
    }
    s21_decimal tmp = {0, 0, 0, 0};
    if (_s21_add_(*result, multiplicand, &tmp) == 1) {
      for (int i = 0; i != 4; i++) {
        result->bits[i] = 0;
      }
      return 1;
    }
    *result = tmp;
    ten_exp.bits[3] |= 1 << 16;
    s21_truncate(ten_exp, &ten_exp);
    ten_exp.bits[3] = 0;
    _s21_sub_(value_2, ten_exp, &value_2);
    ten_exp.bits[0] = 10;
    ten_exp.bits[1] = 0;
    ten_exp.bits[2] = 0;
    ten_exp.bits[3] = 0;
  }
  value_1.bits[3] = 0;
  for (int i = value_2.bits[0]; i > 0; i--) {
    s21_decimal tmp = {0, 0, 0, 0};
    if (_s21_add_(*result, value_1, &tmp) == 1) {
      for (int i = 0; i != 4; i++) {
        result->bits[i] = 0;
      }
      return 1;
    }
    *result = tmp;
  }
  __turn_info_into_decimal__(a_scale + b_scale, sign_a ^ sign_b, result);
  return 0;
}

///////////////////////////////////////////////////////////////

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (value_1.bits[0] == value_2.bits[0] &&
      value_1.bits[1] == value_2.bits[1] &&
      value_1.bits[2] == value_2.bits[2]) {
    result->bits[0] = 1;
    result->bits[1] = 0;
    result->bits[2] = 0;
    result->bits[3] = 0;
    return 0;
  }
  if (check_zero_decimal(value_2)) {
    return 2;
  }
  int scale_factor_a = (value_1.bits[3] >> 16) & 0xFF;
  int scale_factor_b = (value_2.bits[3] >> 16) & 0xFF;
  int sign = (value_1.bits[3] ^ value_2.bits[3]) & 0x80000000;
  int scale_factor = scale_factor_a - scale_factor_b;
  unsigned int remainder = 0;
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
  if (scale_factor < 0) {
    s21_decimal mul = {10, 0, 0, 0};
    for (; scale_factor != 0; scale_factor++) {
      s21_mul(*result, mul, result);
    }
  }
  result->bits[3] = (scale_factor << 16) | sign;
  return 0;
}

int check_zero_decimal(s21_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;
}

///////////////////////////////////////////////////////////////

int s21_is_equal(s21_decimal a, s21_decimal b) {
  if (a.bits[0] == 0 && a.bits[1] == 0 && a.bits[2] == 0 && b.bits[0] == 0 &&
      b.bits[1] == 0 && b.bits[2] == 0) {
    return 1;
  }
  int sign_a = (a.bits[3] >> 31) & 1;
  int sign_b = (b.bits[3] >> 31) & 1;
  if (sign_a != sign_b) {
    return 0;
  }
  normalize_decimal(&a, &b);
  for (int i = 2; i >= 0; i--) {
    if (a.bits[i] != b.bits[i]) {
      return 0;
    }
  }
  return 1;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  if (a.bits[0] == 0 && a.bits[1] == 0 && a.bits[2] == 0 && b.bits[0] == 0 &&
      b.bits[1] == 0 && b.bits[2] == 0) {
    return 0;
  }
  int aIsNegative = (a.bits[3] & 0x80000000);
  int bIsNegative = (b.bits[3] & 0x80000000);
  if (aIsNegative != bIsNegative) {
    return !aIsNegative;
  }
  if (s21_is_equal(a, b)) {
    return 0;
  }
  normalize_decimal(&a, &b);
  for (int i = 2; i >= 0; i--) {
    if (a.bits[i] != b.bits[i]) {
      return (aIsNegative ? a.bits[i] < b.bits[i] : a.bits[i] > b.bits[i]);
    }
  }
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
  s21_truncate(src, &src);
  *dst = 0;
  if (src.bits[1] != 0 || src.bits[2] != 0) {
    return 1;
  } else {
    *dst = src.bits[0];
  }
  *dst *= sign_src == 1 ? -1 : 1;
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
  for (int i = 0; i != 4; i++) {
    dst->bits[i] = 0;
  }
  char buf[40] = "\0";
  sprintf(buf, "%.7g", src);
  int scale = 0;
  int sign = 0;
  unsigned int decimal = 0;
  int factor = 0;
  int count_scale = 0;
  int zeros_counter_in_e_case = 0;
  int zeros_counter = 0;
  for (char *p = buf; *p != '\0'; p++) {
    if (*p != '.' && *p != '-' && *p != 'e') {
      scale += count_scale ? 1 : 0;
      decimal += (*p - 48) * pow(10, factor);
      factor++;
    } else {
      if (*p == '.') {
        count_scale = 1;
      } else if (*p == '-') {
        sign = 1;
      } else if (*p == 'e') {
        int exp = 0;
        int exp_factor = 0;
        for (char *h = p; *h != '\0'; h++) {
          if (*h != '-' && *h != 'e') {
            exp += (*h - 48) * pow(10, exp_factor);
            exp_factor++;
          }
          if (*h == '0') {
            zeros_counter_in_e_case++;
          } else {
            zeros_counter_in_e_case = 0;
          }
        }
        exp = reverse_number(exp);
        for (; zeros_counter_in_e_case != 0; zeros_counter_in_e_case--) {
          exp *= 10;
        }
        scale += exp;
        break;
      }
    }
    if (*p == '0') {
      zeros_counter++;
    } else {
      zeros_counter = 0;
    }
  }
  __turn_info_into_decimal__(scale, sign, dst);
  dst->bits[0] = reverse_number(decimal);
  for (; zeros_counter != 0; zeros_counter--) {
    dst->bits[0] *= 10;
  }
  return 0;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int scale = (src.bits[3] >> 16) & 0xFF;
  int sign = (src.bits[3] >> 31) & 1;
  int return_value = 0;
  *dst = 0.f;
  for (int i = 0; i < 96; i++) {
    if (src.bits[i / 32] & (1U << (i % 32))) {
      *dst += pow(2, i);
    }
  }
  while (scale) {
    *dst /= 10;
    scale--;
  }
  *dst *= sign ? -1 : 1;
  return return_value;
}

/////////////////////////////////////////////////////////////////

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int src_scale = (value.bits[3] >> 16) & 0xFF;
  int sign_src = (value.bits[3] >> 31) & 1;
  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }
  if (src_scale > 28) {
    return 1;
  }
  if (src_scale > 0) {
    s21_decimal divisor = {10, 0, 0, 0};
    for (; src_scale != 0; src_scale--) {
      s21_div(value, divisor, &value);
    }
  }
  *result = value;
  __turn_info_into_decimal__(0, sign_src, result);
  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int sign_src = (value.bits[3] >> 31) & 1;
  int src_scale = (value.bits[3] >> 16) & 0xFF;
  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
    __turn_info_into_decimal__(0, sign_src, result);
    return 0;
  }
  if (src_scale == 0) {
    for (int i = 0; i != 4; i++) {
      result->bits[i] = value.bits[i];
    }
    return 0;
  }
  if (s21_truncate(value, result) == 1) {
    return 1;
  }
  if (sign_src == 1) {
    s21_decimal minus_one = {1, 0, 0, 0};
    minus_one.bits[3] |= 0x80000000;
    s21_decimal tmp = {0, 0, 0, 0};
    _s21_add_(*result, minus_one, &tmp);
    *result = tmp;
    __turn_info_into_decimal__(0, 1, result);
  }
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int src_scale = (value.bits[3] >> 16) & 0xFF;
  int sign_src = (value.bits[3] >> 31) & 1;
  for (int i = 0; i != 4; i++) {
    result->bits[i] = 0;
  }
  s21_decimal res = {0, 0, 0, 0};
  if (src_scale > 0) {
    s21_decimal tmp = value;
    __turn_info_into_decimal__(src_scale - 1, sign_src, &tmp);
    if (s21_truncate(tmp, &tmp) == 1) {
      return 1;
    }
    if (s21_truncate(value, &value) == 1) {
      return 1;
    }
    __turn_info_into_decimal__(1, sign_src, &tmp);
    s21_sub(tmp, value, &tmp);
    if (tmp.bits[0] >= 5) {
      s21_decimal one = {1, 0, 0, 0};
      _s21_add_(value, one, result);
      __turn_info_into_decimal__(0, sign_src, result);
    } else {
      *result = value;
      __turn_info_into_decimal__(0, sign_src, result);
    }
  } else {
    *result = value;
  }
  return 0;
}
