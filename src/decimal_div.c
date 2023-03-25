#include "decimal.h"

// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//   _Bool __binary1__[96] = {false};
//   _Bool __binary2__[96] = {false};

//   for (int i = 0; i != 3; i++) {
//     perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__); //
//     perform_decimal_into_binary(value_2.bits[i], i + 1, __binary2__); //
//   }

//   _Bool __result__[96] = {false};

//   int position = poly_div(__binary1__, __binary2__, __result__, 0);
//   __turn_info_into_decimal__(position, 0, result); //
//   convert_binary_into_decimal(__result__, result); //

//   return 0;
// }

//////////////////////////////////////////////

void convert_binary_into_decimal(_Bool *value, s21_decimal *result) {

  for (int shift = 1; shift != 4; shift++) {
    int decimal = 0;
    for (int i = 0; i != 32; i++) {
      decimal += value[shift * 32 - i - 1] * pow(2, i);
    }
    result->bits[shift - 1] = decimal;
  }
}

///////////////////////////////////////////////

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

/////////////////////////////////////////////////

int poly_div(_Bool *binary1, _Bool *binary2, _Bool *result, int mod_flag) {

  int position = 0;
  _Bool zero[96] = {0};
  if (!__s21_is_equal__(binary2, zero)) {                //
    while (!__s21_is_equal__(binary1, zero)) {           //
      position = take_element(binary1, binary2, result); //
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
    convert_binary_into_decimal(tmp_bool, &tmp_dec); //

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

//////////////////////////////////////////////////////

void __div_decimal_convert__(s21_decimal value_1, int *decimal) {
  _Bool __binary1__[96] = {false};
  for (int i = 0; i != 3; i++) {
    perform_decimal_into_binary(value_1.bits[i], i + 1, __binary1__); //
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

//////////////////////////////////////////////////////

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
  while (!__s21_is_greater_or_equal__(tmp, binary2)) { //
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

    __s21_sub__(binary1, tmp, binary1); //

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
  if (__s21_is_equal__(binary1, zero) && mask != 96) { //
    for (int i = mask; i != 96; i++) {
      for (int i = 0; i != 96; i++) {
        result[i] = result[i + 1];
      }
      result[95] = 0;
    }
  }

  return point_pos;
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

///////////////////////////////////////////////////////////////////

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