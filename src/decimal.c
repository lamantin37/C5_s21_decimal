#include "decimal.h"
int main() {
    _Bool __binary__[96] = {false};
    perform_decimal_into_binary(2147483647, 3, __binary__);
    perform_decimal_into_binary(2147483647, 2, __binary__);
    perform_decimal_into_binary(2147483647, 1, __binary__);
    for (int i = 0; i != 96; i++) {
        printf("%d\n", __binary__[i]);
    }
    return 0;
}

void perform_decimal_into_binary(int number, int bits, _Bool* __binary__) {
    int shift = bits * 32 - __predict_len__(number) - 1;
    __converter__(number, &__binary__[shift]);
}

void __converter__(int number, _Bool* __binary__) {
    int shift = __predict_len__(number);
    for (; number > 1; ) {
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