#include "decimal.h"

// < //
// <= //
// > //
// >= //
// == //
// != //

int main() {
  // perform_decimal_into_binary(2023, 3, __binary__);
  // perform_decimal_into_binary(1523479, 2, __binary__);
  // perform_decimal_into_binary(-2147483648, 1, __binary__);
  // for (int i = 0; i != 96; i++) {
  //     printf("%d\n", __binary__[i]);
  // }
  // s21_mul(p1, p2, &p3);



  // s21_decimal p1 = {0, 0, 23, 65536};  // 1.373 2
  // s21_decimal p2 = {0, 0, 43, 131072}; // 1 0
  // s21_decimal result = {0, 0, 0, 0};

  // s21_add(p1, p2, &result);
  // printf("%d\n", result.bits[0]);
  // printf("%d\n", result.bits[1]);
  // printf("%d\n", result.bits[2]);
  // printf("%d\n\n", result.bits[3]);


  // float dst = 0;
  // s21_from_decimal_to_float(result, &dst);
  // printf("%f\n", dst);

  s21_decimal result = {0};

  float a = 123345764.1;
  s21_from_float_to_decimal(a, &result);

  printf("%d\n", result.bits[0]);
  printf("%d\n", result.bits[1]);
  printf("%d\n", result.bits[2]);
  printf("%d\n", result.bits[3]);
  

  // printf("%d\n", s21_is_greater(p1, p2));

  // 10    1100011011010100000011011110
  // 1000001100011011010100000011011110
  return 0;
}

void perform_decimal_into_binary(int number, int bits, _Bool *__binary__) {
  number = number < 0 ? number * -1 : number;
  __binary__[(bits - 1) * 32] = (number == MINIMUM_INT ? 1 : 0);
  if (number == MINIMUM_INT) {
    for (int i = 1; i != 32; i++) {
      __binary__[i + (bits - 1) * 32] = 1;
    }
  } else {
    int shift = bits * 32 - __predict_len__(number) - 1;
    __converter__(number, &__binary__[shift]);
  }
}

void __converter__(int number, _Bool *__binary__) {
  int shift = __predict_len__(number);
  for (; number > 1;) {
    __binary__[shift - __predict_len__(number)] = 1;
    number -= (int)pow(2, __predict_len__(number));
  }
  __binary__[shift] = number;
}

int __predict_len__(int number) {
  int lenght = 0;
  for (; (int)pow(2, lenght) <= number; lenght++) {
    if (lenght == 31) {
      break;
    }
  }
  return --lenght;
}

void convert_binary_into_decimal(_Bool *value, s21_decimal *result) {

  for (int shift = 1; shift != 4; shift++) {
    int decimal = 0;
    for (int i = 0; i != 32; i++) {
      decimal += value[shift * 32 - i - 1] * pow(2, i);
    }
    result->bits[shift - 1] = decimal;
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  _Bool __result__[96] = {false};

  s21_decimal_info info_1 = {0};
  s21_decimal_info info_2 = {0};

  __take_info__(&info_1, value_1);
  __take_info__(&info_2, value_2);

  // Возможно придется фиксить

  result->bits[3] =
      info_1.position > info_2.position ? value_1.bits[3] : value_2.bits[3];

  // Возможно придется фиксить

  if (info_1.position != 0 || info_2.position != 0) {
    if (info_1.position > info_2.position) {
      for (int i = info_1.position - info_2.position; i != 96; i++) {
        __binary2__[i] = __binary2__[i + (info_1.position - info_2.position)];
      }
    } else if (info_1.position < info_2.position) {

      for (int i = info_2.position - info_1.position; i != 96; i++) {
        __binary1__[i] = __binary1__[i + (info_2.position - info_1.position)];
      }
    }
  }

  __s21_add__(__binary1__, __binary2__, __result__);

  convert_binary_into_decimal(__result__, result);
}

void __s21_add__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__) {
  int k, p = 0;
  for (int i = 95; i != -1; i--) {
    k = __binary1__[i] + __binary2__[i] + p;
    __result__[i] = k % 2;
    p = k / 2;
  }
  if (p > 0) {
    __result__[0] = p;
  }
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  _Bool __result__[96] = {false};
  __s21_sub__(__binary1__, __binary2__, __result__);

  convert_binary_into_decimal(__result__, result);
}

void __s21_sub__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__) {

  int k, p = 0;
  for (int i = 95; i != -1; i--) {
    k = __binary1__[i] - __binary2__[i] + p;
    p = k == 2 ? 0 : p;
    k = k == 2 ? 0 : k;
    p = k < 0 ? 1 : k == 2 ? 0 : p;
    k = k < 0 ? k * -1 : k;
    __result__[i] = k;
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  _Bool __result__[96] = {false};
  int i = 0;
  for (; __binary2__[i] != 1; i++) {
    if (i == 95) {
      break;
    }
  }

  if (__binary2__[95]) {
    __s21_add__(__result__, __binary1__, __result__);
  }

  for (int cnt = 1; cnt != 96 - i; cnt++) {

    for (int j = cnt; j != 96; j++) {
      __binary1__[j - 1] = __binary1__[j];
    }
    __binary1__[96 - cnt] = 0;

    if (__binary2__[95 - cnt]) {
      __s21_add__(__result__, __binary1__, __result__);
    }
  }

  convert_binary_into_decimal(__result__, result);
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  _Bool __result__[96] = {false};

  __s21_div__(__binary1__, __binary2__, __result__);

  convert_binary_into_decimal(__result__, result);
}

int __s21_div__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__) {
  while (__s21_is_greater_or_equal__(__binary1__, __binary2__, 1)) {
    __s21_sub__(__binary1__, __binary2__, __binary1__);
    _Bool __one__[96] = {false};
    __one__[95] = 1;
    __s21_add__(__result__, __one__, __result__);
  }
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  _Bool __result__[96] = {false};

  __s21_div__(__binary1__, __binary2__, __result__);

  convert_binary_into_decimal(__binary1__, result);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};

  int return_value = 1;

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  return_value =
      __s21_is_greater_or_equal__(__binary1__, __binary2__, return_value);

  return return_value;
}

int __s21_is_greater_or_equal__(_Bool *__binary1__, _Bool *__binary2__,
                                int return_value) {

  if ((return_value =
           __s21_is_equal__(__binary1__, __binary2__, return_value)) == 0) {
    int zero_flag = 0;
    for (int i = 0; i != 96; i++) {

      if (__binary1__[i] != __binary2__[i]) {
        if (__binary1__[i] == 1) {
          return_value = 1;
        }
        break;
      }
    }
  }

  return return_value;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};

  int return_value = 1;

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  return_value = __s21_is_equal__(__binary1__, __binary2__, return_value);

  return return_value;
}

int __s21_is_equal__(_Bool *__binary1__, _Bool *__binary2__, int return_value) {
  for (int i = 0; i != 96; i++) {
    if (__binary1__[i] != __binary2__[i]) {
      return_value = 0;
      break;
    }
  }
  return return_value;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int return_value = s21_is_greater_or_equal(value_1, value_2) ? 0 : 1;
  return return_value;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  int return_value = s21_is_equal(value_1, value_2) ? 0 : 1;
  return return_value;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int return_value =
      (s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2)) ? 1 : 0;
  return return_value;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int return_value = s21_is_less_or_equal(value_1, value_2) ? 0 : 1;
  return return_value;
}

void __take_info__(s21_decimal_info *info, s21_decimal value_1) {
  _Bool __binary1__[32] = {false};

  perform_decimal_into_binary(value_1.bits[3], 1, __binary1__);

  info->minus = __binary1__[1] ? 1 : 0;

  for (int i = 0; i != 8; i++) {
    info->position += __binary1__[9 + i] * (7 - i);
  }
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  s21_decimal_info info_1 = {0};
  __take_info__(&info_1, src);


  _Bool __binary1__[96] = {false};
  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(src.bits[i], i + 1, __binary1__);
  }

  int cnt = 0;
  for (int i = 0; i != 96; i++, cnt++) {
    if (__binary1__[i]) {
      break;
    }
  }

  for (int i = 95 - cnt - info_1.position; i >= info_1.position * -1; i--) {
    *dst += __binary1__[cnt] * pow(2, i);
    cnt++;
  }
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {

  int million = 1000000;
  int shift = 0;

  if (src < million * 10) {
    while (src < million) {
      src *= 10;
      shift++;
    }
  } else {
    while (src > million * 10) {
      src /= 10;
    }
  }

  if ((src - (int) src) >= 0.5) {
    src += 1.0;
  }

  s21_from_int_to_decimal((int) src, dst);

}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {

  // надо добавить минус

  dst -> bits[2] = src;

  return 0;
}