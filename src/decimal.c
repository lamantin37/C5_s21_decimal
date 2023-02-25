#include "decimal.h"

// < //
// <= //
// > //
// >= //
// == //
// != //

int main() {

  s21_decimal p1 = {0, 0, 10000, 0};
  s21_decimal p2 = {0, 0, 1, 0};
  s21_decimal result = {0, 0, 0, 0};

  // полохо обрабатывает в случаях с нулевой точностью одного из децимал
  __turn_info_into_decimal__(3, 0, &p1);
  __turn_info_into_decimal__(0, 0, &p2);

  // s21_add(p1, p2, &result);
  s21_truncate(p1, &result);

  printf("%u\n", result.bits[0]);
  printf("%u\n", result.bits[1]);
  printf("%u\n", result.bits[2]);
  printf("%u\n", result.bits[3]);

  // int _int[30] = {0};
  // _int[29] = 7;
  // _int[28] = 2;
  // _int[27] = 2;

  // int __div_decimal__[30] = {0};
  // __div_decimal__[29] = 8;
  // __div_decimal__[28] = 2;
  // __div_decimal__[27] = 1;
  // ______div_decimal_sub______(_int, __div_decimal__, _int);

  // for (int i = 0; i != 30; i++) {
  //   printf("%d", _int[i]);
  // }
  // printf("\n");

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

  s21_decimal_info info_1 = {0};
  __take_info__(&info_1, value_1);
  s21_decimal_info info_2 = {0};
  __take_info__(&info_2, value_2);

  if (info_1.position == 0 && info_2.position == 0) {
    _Bool __binary1__[96] = {false};
    _Bool __binary2__[96] = {false};
    _Bool __result__[96] = {false};
    for (int i = 0; i != 3; i++) {
      perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
      perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
    }
    __s21_add__(__binary1__, __binary2__, __result__);
    convert_binary_into_decimal(__result__, result);
  } else {
    s21_decimal _int_1 = {0, 0, 0, 0};
    s21_decimal _point_1 = {0, 0, 0, 0};
    s21_decimal _int_2 = {0, 0, 0, 0};
    s21_decimal _point_2 = {0, 0, 0, 0};

    s21_decimal _result_int = {0, 0, 0, 0};
    s21_decimal _result_point = {0, 0, 0, 0};

    if (info_1.position != 0) {
      __div_decimal__(value_1, &_int_1, &_point_1,
                      info_1.position > info_2.position ? info_1.position
                                                        : info_2.position);
    } else {
      _int_1 = value_1;
    }
    if (info_2.position != 0) {
      __div_decimal__(value_2, &_int_2, &_point_2,
                      info_1.position > info_2.position ? info_1.position
                                                        : info_2.position);
    } else {
      _int_2 = value_2;
    }

    // convert int part of decimal

    _Bool __binary1__[96] = {false};
    _Bool __binary2__[96] = {false};
    _Bool __result_int__[96] = {false};
    for (int i = 0; i != 3; i++) {
      perform_decimal_into_binary(_int_1.bits[i], i + 1, __binary1__);
      perform_decimal_into_binary(_int_2.bits[i], i + 1, __binary2__);
    }
    __s21_add__(__binary1__, __binary2__, __result_int__);
    convert_binary_into_decimal(__result_int__, &_result_int);

    for (int i = 0; i != 96; i++) {
      __binary1__[i] = 0;
      __binary2__[i] = 0;
    }

    // convert after-point part of decimal

    _Bool __result_point__[96] = {false};
    for (int i = 0; i != 3; i++) {
      perform_decimal_into_binary(_point_1.bits[i], i + 1, __binary1__);
      perform_decimal_into_binary(_point_2.bits[i], i + 1, __binary2__);
    }

    result->bits[3] =
        info_1.position > info_2.position ? value_1.bits[3] : value_2.bits[3];

    __s21_add__(__binary1__, __binary2__, __result_point__);
    convert_binary_into_decimal(__result_point__, &_result_point);

    // convert two decimals into binary format

    for (int i = 0; i != 96; i++) {
      __binary1__[i] = 0;
      __binary2__[i] = 0;
    }

    for (int i = 0; i != 3; i++) {
      perform_decimal_into_binary(_result_int.bits[i], i + 1, __binary1__);
      perform_decimal_into_binary(_result_point.bits[i], i + 1, __binary2__);
    }

    // covert two binary arrays into two 30int arrays

    int decimal_int[30] = {0};
    int temp_double[30] = {0};
    for (int i = 0; i != 96; i++) {
      if (__binary1__[95 - i]) {
        for (int i = 0; i != 30; i++) {
          temp_double[i] = 0;
        }
        temp_double[29] = 2;
        ____div_decimal_pow____(temp_double, i, temp_double);
        ______div_decimal_add______(decimal_int, temp_double, decimal_int);
      }
    }

    int decimal_point[30] = {0};
    for (int i = 0; i != 96; i++) {
      if (__binary2__[95 - i]) {
        for (int i = 0; i != 30; i++) {
          temp_double[i] = 0;
        }
        temp_double[29] = 2;
        ____div_decimal_pow____(temp_double, i, temp_double);
        ______div_decimal_add______(decimal_point, temp_double, decimal_point);
      }
    }

    // convert two 30int arrays into an array of b10 notation, representation of
    // decimal

    _Bool _result_position_array[32] = {false};
    perform_decimal_into_binary(result->bits[3], 1, _result_position_array);
    int position_result = 0;
    for (int i = 0; i != 8; i++) {
      position_result += _result_position_array[9 + i] * (7 - i);
    }

    // rounding in case point + point surpassed one

    if (decimal_point[29 - position_result] != 0) {
      decimal_point[29 - position_result] = 0;
      int _one[30] = {0};
      _one[29] = 1;
      ______div_decimal_add______(decimal_int, _one, decimal_int);
    }

    // keep convertin'

    __div_convert_back(decimal_int, decimal_point, result, position_result);
  }
}

//

void __div_convert_back(int *decimal_int, int *decimal_point,
                        s21_decimal *result, int position_result) {
  int _b10_notation[30] = {0};
  for (int i = 0; i != 30; i++) {
    if (i <= 29 - position_result) {
      _b10_notation[i] = decimal_int[i + position_result];
    } else {
      _b10_notation[i] = decimal_point[i];
    }
  }

  _Bool __binary_result[96] = {0};
  __div_perform_back__(_b10_notation, __binary_result);

  convert_binary_into_decimal(__binary_result, result);
}

//

void __s21_add__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__) {
  int k, p = 0;
  for (int i = 95; i != -1; i--) {
    k = __binary1__[i] + __binary2__[i] + p;
    __result__[i] = k % 2;
    p = k / 2;
  }
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

  s21_decimal_info info_1 = {0};
  __take_info__(&info_1, value_1);
  s21_decimal_info info_2 = {0};
  __take_info__(&info_2, value_2);

  if (info_1.position == 0 && info_2.position == 0) {
    _Bool __binary1__[96] = {false};
    _Bool __binary2__[96] = {false};
    _Bool __result__[96] = {false};
    for (int i = 0; i != 3; i++) {
      perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
      perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
    }
    __s21_sub__(__binary1__, __binary2__, __result__);
    convert_binary_into_decimal(__result__, result);
  } else {
    s21_decimal _int_1 = {0, 0, 0, 0};
    s21_decimal _point_1 = {0, 0, 0, 0};
    s21_decimal _int_2 = {0, 0, 0, 0};
    s21_decimal _point_2 = {0, 0, 0, 0};

    s21_decimal _result_int = {0, 0, 0, 0};
    s21_decimal _result_point = {0, 0, 0, 0};

    if (info_1.position != 0) {
      __div_decimal__(value_1, &_int_1, &_point_1,
                      info_1.position > info_2.position ? info_1.position
                                                        : info_2.position);
    } else {
      _int_1 = value_1;
    }
    if (info_2.position != 0) {
      __div_decimal__(value_2, &_int_2, &_point_2,
                      info_1.position > info_2.position ? info_1.position
                                                        : info_2.position);
    } else {
      _int_2 = value_2;
    }

    // check if _point_1 required one more 1

    if (s21_is_less(_point_1, _point_2)) {
      int decimal_point[30] = {0};
      __div_decimal_convert__(_point_1, decimal_point);

      for (int i = 0; i != 30; i++) {
        if (decimal_point[i] != 0) {
          decimal_point[i - 1] = 1;
          break;
        }
      }

      _Bool _temp_binary_decimal[96] = {0};
      __div_perform_back__(decimal_point, _temp_binary_decimal);
      convert_binary_into_decimal(_temp_binary_decimal, &_point_1);

      _Bool _binary_decimal_int_1[96] = {0};
      _Bool _binary_decimal_int_2[96] = {0};
      _binary_decimal_int_2[95] = 1;
      for (int i = 0; i != 3; i++) {
        perform_decimal_into_binary(_int_1.bits[i], i + 1,
                                    _binary_decimal_int_1);
      }
      __s21_sub__(_binary_decimal_int_1, _binary_decimal_int_2,
                  _binary_decimal_int_1);
      convert_binary_into_decimal(_binary_decimal_int_1, &_int_1);
    }

    // convert int part of decimal

    _Bool __binary1__[96] = {false};
    _Bool __binary2__[96] = {false};
    _Bool __result_int__[96] = {false};
    for (int i = 0; i != 3; i++) {
      perform_decimal_into_binary(_int_1.bits[i], i + 1, __binary1__);
      perform_decimal_into_binary(_int_2.bits[i], i + 1, __binary2__);
    }
    __s21_sub__(__binary1__, __binary2__, __result_int__);
    convert_binary_into_decimal(__result_int__, &_result_int);

    for (int i = 0; i != 96; i++) {
      __binary1__[i] = 0;
      __binary2__[i] = 0;
    }

    // convert after-point part of decimal

    _Bool __result_point__[96] = {false};
    for (int i = 0; i != 3; i++) {
      perform_decimal_into_binary(_point_1.bits[i], i + 1, __binary1__);
      perform_decimal_into_binary(_point_2.bits[i], i + 1, __binary2__);
    }

    result->bits[3] =
        info_1.position > info_2.position ? value_1.bits[3] : value_2.bits[3];

    __s21_sub__(__binary1__, __binary2__, __result_point__);
    convert_binary_into_decimal(__result_point__, &_result_point);

    // convert two decimals into binary format

    for (int i = 0; i != 96; i++) {
      __binary1__[i] = 0;
      __binary2__[i] = 0;
    }

    for (int i = 0; i != 3; i++) {
      perform_decimal_into_binary(_result_int.bits[i], i + 1, __binary1__);
      perform_decimal_into_binary(_result_point.bits[i], i + 1, __binary2__);
    }

    // covert two binary arrays into two 30int arrays

    int decimal_int[30] = {0};
    int temp_double[30] = {0};
    for (int i = 0; i != 96; i++) {
      if (__binary1__[95 - i]) {
        for (int i = 0; i != 30; i++) {
          temp_double[i] = 0;
        }
        temp_double[29] = 2;
        ____div_decimal_pow____(temp_double, i, temp_double);
        ______div_decimal_add______(decimal_int, temp_double, decimal_int);
      }
    }

    int decimal_point[30] = {0};
    for (int i = 0; i != 96; i++) {
      if (__binary2__[95 - i]) {
        for (int i = 0; i != 30; i++) {
          temp_double[i] = 0;
        }
        temp_double[29] = 2;
        ____div_decimal_pow____(temp_double, i, temp_double);
        ______div_decimal_add______(decimal_point, temp_double, decimal_point);
      }
    }

    // convert two 30int arrays into an array of b10 notation, representation of
    // decimal

    _Bool _result_position_array[32] = {false};
    perform_decimal_into_binary(result->bits[3], 1, _result_position_array);
    int position_result = 0;
    for (int i = 0; i != 8; i++) {
      position_result += _result_position_array[9 + i] * (7 - i);
    }

    // keep convertin'

    __div_convert_back(decimal_int, decimal_point, result, position_result);
  }
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

  s21_decimal_info info_1 = {0};
  __take_info__(&info_1, value_1);
  s21_decimal_info info_2 = {0};
  __take_info__(&info_2, value_2);
  __turn_info_into_decimal__(info_1.position + info_2.position, 0, result);

  __s21_mul__(__binary1__, __binary2__, __result__);
  convert_binary_into_decimal(__result__, result);
}

void __s21_mul__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__) {

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
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  _Bool __result__[96] = {false};

  // __s21_div__(__binary1__, __binary2__, __result__);
  int position = poly_div(__binary1__, __binary2__, __result__, 0);
  __turn_info_into_decimal__(position, 0, result);
  convert_binary_into_decimal(__result__, result);
}

//////////////////////////////////////////////////////////////////

int take_element(_Bool *binary1, _Bool *binary2, _Bool *result) {

  int max_i = 23;

  const int first_step = 111111;

  // should be cleared //

  static int lenght = first_step;
  static int max_iter = 0;
  static int point_pos = 0;
  static int position_flag = 0;

  // should be cleared //

  _Bool tmp[96] = {0};
  _Bool iter = 0;

  int mask = 0;

  for (int i = 0; i != 96; i++) {
    if (binary1[i] == 1) {
      break;
    }
    mask++;
  }

  mask -= lenght < 0 ? lenght * -1 : 0;
  lenght = lenght < 0 ? 0 : lenght;

  int counter = -1;
  while (!__s21_is_greater_or_equal__(tmp, binary2)) {
    if (mask == 96) {
      position_flag = 1;
      if (lenght == first_step) {
        for (int i = 0; i != 96; i++) {
          result[i] = result[i + 1];
        }
        result[95] = 0;
        point_pos += position_flag ? 1 : 0;
      }
      if (max_iter == max_i) {
        for (int i = 0; i != 96; i++) {
          binary1[i] = 0;
        }
        break;
      }
      for (int i = 0; i != 96; i++) {
        binary1[i] = binary1[i + 1];
      }
      mask--;
      max_iter = iter ? max_iter : ++max_iter, iter = 1;
    }
    counter++;
    for (int i = 0; i != 95; i++) {
      tmp[i] = tmp[i + 1];
    }
    tmp[95] = binary1[mask++];

    if (counter >= lenght + 1) {
      for (int i = 0; i != 96; i++) {
        result[i] = result[i + 1];
      }
      result[95] = 0;
      point_pos += position_flag ? 1 : 0;
    }
  }

  if (max_iter != max_i) {

    for (int i = 0; i != 96; i++) {
      if (i < mask) {
        tmp[i] = binary2[i + (96 - mask)];
      } else {
        tmp[i] = 0;
      }
    }

    __s21_sub__(binary1, tmp, binary1);

    for (int i = 0; i != 96; i++) {
      result[i] = result[i + 1];
    }
    result[95] = 1;
    point_pos += position_flag ? 1 : 0;

    lenght = 0;
    for (; lenght != 96; lenght++) {
      if (binary1[lenght] == 1) {
        break;
      }
    }
    lenght = mask - lenght;
  } else {
    for (int i = 0; i != 96; i++) {
      binary1[i] = 0;
    }
  }

  _Bool zero[96] = {0};
  if (__s21_is_equal__(binary1, zero) && mask != 96) {
    for (int i = mask; i != 96; i++) {
      for (int i = 0; i != 96; i++) {
        result[i] = result[i + 1];
      }
      result[95] = 0;
    }
  }

  return point_pos;
}

int poly_div(_Bool *binary1, _Bool *binary2, _Bool *result, int mod_flag) {

  int position = 0;
  _Bool zero[96] = {0};
  if (!__s21_is_equal__(binary2, zero)) {
    while (!__s21_is_equal__(binary1, zero)) {
      position = take_element(binary1, binary2, result);
    }
  }

  _Bool tmp_bool[96] = {0};
  for (int i = 96 - position; i != 96; i++) {
    tmp_bool[i] = result[i];
  }

  int point_position = 29;
  if (mod_flag) {
    for (int i = 0; i != 96; i++) {
      result[i] = tmp_bool[i];
    }
    point_position = 0;
  } else {
    s21_decimal tmp_dec = {0};
    convert_binary_into_decimal(tmp_bool, &tmp_dec);

    int tmp_partition_dec[30] = {0};
    __div_decimal_convert__(tmp_dec, tmp_partition_dec);

    for (int i = 0; i != 30; i++) {
      if (tmp_partition_dec[i] != 0) {
        break;
      }
      point_position--;
    }
  }
  return point_position;
}

//////////////////////////////////////////////////////////////////

int __s21_div__(_Bool *__binary1__, _Bool *__binary2__, _Bool *__result__) {
  while (__s21_is_greater_or_equal__(__binary1__, __binary2__)) {
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

  poly_div(__binary1__, __binary2__, __result__, 1);
  convert_binary_into_decimal(__result__, result);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};

  int return_value = 1;

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  return __s21_is_greater_or_equal__(__binary1__, __binary2__);
}

int __s21_is_greater_or_equal__(_Bool *__binary1__, _Bool *__binary2__) {
  int return_value = 0;
  if ((return_value = __s21_is_equal__(__binary1__, __binary2__)) == 0) {
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

  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__);
    perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__);
  }

  return __s21_is_equal__(__binary1__, __binary2__);
}

int __s21_is_equal__(_Bool *__binary1__, _Bool *__binary2__) {
  int return_value = 1;
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

  //

  int minus = 0;

  //

  if (src < million * 10) {
    while (src < million) {
      src *= 10;
      shift++;
    }

    src += (src - (int)src) >= 0.5 ? 1 : 0;

    while ((int)src % 10 == 0) {
      src /= 10;
      shift--;
    }

  } else {
    while (src > million * 10) {
      src /= 10;
    }
    src += (src - (int)src) >= 0.5 ? 1 : 0;
  }

  s21_from_int_to_decimal((int)src, dst);

  __turn_info_into_decimal__(shift, minus, dst);
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {

  // надо добавить минус

  dst->bits[2] = src;

  return 0;
}

void __turn_info_into_decimal__(int position, int minus, s21_decimal *dst) {
  _Bool __binary1__[32] = {false};

  for (int i = 7; i >= 0; i--) {
    __binary1__[16 - i] = (position - i >= 0) ? 1 : 0;
    position = (position - i >= 0) ? position - i : position;
  }

  __binary1__[1] = minus ? 1 : 0;

  int decimal = 0;

  for (int i = 31; i >= 0; i--) {
    decimal += __binary1__[i] * pow(2, 31 - i);
  }
  dst->bits[3] = decimal;
}

void rounding(s21_decimal value, s21_decimal *result) {
  int tmp[30] = {0};
  __div_decimal_convert__(value, tmp);
  tmp[0] = 0;
  if (tmp[29] >= 5) {
    for (int i = 29; i != 0; i--) {
      tmp[i] = tmp[i - 1];
    }
    int one[30] = {0};
    one[29] = 1;
    ______div_decimal_add______(tmp, one, tmp);
  } else {
    for (int i = 29; i != 0; i--) {
      tmp[i] = tmp[i - 1];
    }
  }
  _Bool _temp_binary_decimal[96] = {0};
  __div_perform_back__(tmp, _temp_binary_decimal);
  convert_binary_into_decimal(_temp_binary_decimal, result);
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  s21_truncate(value, result);
}

int s21_round(s21_decimal value, s21_decimal *result) {
  s21_decimal_info info_1 = {0};
  __take_info__(&info_1, value);
  while (info_1.position > 0) {
    rounding(value, &value);
    info_1.position--;
  }

  for (int i = 0; i != 3; i++) {
    result -> bits[i] = value.bits[i];
  }
  __turn_info_into_decimal__(0, info_1.minus, result);
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_decimal _point_1 = {0};
  __div_decimal__(value, result, &_point_1, 0);
}