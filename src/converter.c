#include "decimal.h"

void __div_decimal__(s21_decimal value_1, s21_decimal *__int__,
                     s21_decimal *__point__, int position) {

  int decimal[30] = {0};
  __div_decimal_convert__(value_1, decimal);

  _Bool _temp_binary_decimal[96] = {0};
  int temp_decimal[30] = {0};
  s21_decimal_info info = {0};
  __take_info__(&info, value_1);
  for (int i = 0; i < 30 - info.position; i++) {
    temp_decimal[i + info.position] = decimal[i];
  }

  __div_perform_back__(temp_decimal, _temp_binary_decimal);
  convert_binary_into_decimal(_temp_binary_decimal, __int__);

  for (int i = 0; i != 30; i++) {
    temp_decimal[i] = 0;
  }

  for (int i = 0; i != 96; i++) {
    _temp_binary_decimal[i] = 0;
  }

  for (int i = 30 - info.position; i != 30; i++) {
    temp_decimal[i] = decimal[i];
  }

  if (position > info.position) {
    for (int i = info.position; i != 30; i++) {
      temp_decimal[i - (position - info.position)] = temp_decimal[i];
      temp_decimal[i] = 0;
    }
  }

  __div_perform_back__(temp_decimal, _temp_binary_decimal);
  convert_binary_into_decimal(_temp_binary_decimal, __point__);
  __turn_info_into_decimal__(info.position, 0, __point__);
}

void __div_decimal_convert__(s21_decimal value_1, int *decimal) {
  _Bool __binary1__[96] = {false};
  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
  }
  int temp_double[30] = {0};
  for (int i = 0; i != 96; i++) {
    if (__binary1__[95 - i]) {
      for (int i = 0; i != 30; i++) {
        temp_double[i] = 0;
      }
      temp_double[29] = 2;
      ____div_decimal_pow____(temp_double, i, temp_double);
      ______div_decimal_add______(decimal, temp_double, decimal);
    }
  }
}

void ____div_decimal_pow____(int *__int1__, int power, int *__result__) {
  if (power) {
    for (int i = 0; i < power - 1; i++) {
      ______div_decimal_add______(__int1__, __int1__, __int1__);
    }
    for (int j = 0; j != 30; j++) {
      __result__[j] = __int1__[j];
    }
  } else {
    __result__[29] = 1;
  }
}

void ______div_decimal_add______(int *__int1__, int *__int2__,
                                 int *__result__) {
  int k = 0;
  int buf[30] = {0};
  for (int i = 29; i >= 0; i--) {
    buf[i] = __int1__[i] + __int2__[i] + k >= 10
                 ? (__int1__[i] + __int2__[i] + k) % 10
                 : __int1__[i] + __int2__[i] + k;
    k = __int1__[i] + __int2__[i] + k >= 10 ? 1 : 0;
  }
  for (int i = 0; i != 30; i++) {
    __result__[i] = buf[i];
  }
}

void ______div_decimal_sub______(int *__int1__, int *__int2__,
                                 int *__result__) {
  int k = 0;
  int buf[30] = {0};
  for (int i = 29; i >= 0; i--) {
    buf[i] = (__int1__[i] - __int2__[i] - k) >= 0
                 ? __int1__[i] - __int2__[i] - k
                 : (10 + __int1__[i]) - __int2__[i] - k;
    k = __int1__[i] - __int2__[i] - k < 0 ? 1 : 0;
  }

  for (int i = 0; i != 30; i++) {
    __result__[i] = buf[i];
  }
}

int __div_decimal_higher_or_equal__(int *__int1__, int *__int2__) {
  int return_value = 0;
  for (int i = 0; i != 30; i++) {
    if (__int1__[i] == __int2__[i] && i == 29) {
      return_value = 1;
      break;
    }
    if ((__int1__[i] && __int2__[i]) != 0) {
      return_value = __int1__[i] > __int2__[i] ? 1 : 0;
      if (__int1__[i] == __int2__[i]) {
        continue;
      }
      break;
    } else if (__int1__[i] != 0 || __int2__[i] != 0) {
      return_value = __int1__[i] != 0 ? 1 : 0;
      break;
    }
  }
  return return_value;
}

void __div_perform_back__(int *__int1__, _Bool *__binary1__) {

  int div_decimal[30] = {0};
  for (int i = 0; i != 96; i++) {
    for (int j = 0; j != 30; j++) {
      div_decimal[j] = 0;
    }
    div_decimal[29] = 2;
    ____div_decimal_pow____(div_decimal, 95 - i, div_decimal);
    if (__div_decimal_higher_or_equal__(__int1__, div_decimal)) {
      __binary1__[i] = 1;
      ______div_decimal_sub______(__int1__, div_decimal, __int1__);
    }
  }
}