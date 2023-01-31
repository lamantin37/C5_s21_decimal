#include "decimal.h"
int main() {
  s21_decimal p1, p2, p3;
  // perform_decimal_into_binary(2023, 3, __binary__);
  // perform_decimal_into_binary(1523479, 2, __binary__);
  // perform_decimal_into_binary(-2147483648, 1, __binary__);
  // for (int i = 0; i != 96; i++) {
  //     printf("%d\n", __binary__[i]);
  // }
  s21_add(p1, p2, &p3);
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

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _Bool __binary1__[96] = {false};
  _Bool __binary2__[96] = {false};
  _Bool __result__[96] = {false};
  perform_decimal_into_binary(2023, 3, __binary1__);
  perform_decimal_into_binary(153423, 3, __binary2__);
  int i, k, p = 0;
  for (int i = 95; i != -1; i--) {
    k = __binary1__[i] + __binary2__[i] + p;
    __result__[i] = k % 2;
    p = k / 2;
  }
  if (p > 0) {
    __result__[0] = p;
  }
  for (int i = 0; i != 96; i++) {
    printf("%d\n", __result__[i]);
  }
}
