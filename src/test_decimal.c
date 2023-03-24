#include <stdio.h>

typedef struct {
  int bits[4];
} s21_decimal;

void multiply_by_power_of_10(s21_decimal *decimal) {
  // Multiply the decimal by 10
  unsigned int carry = 0;
  for (int i = 2; i != -1; i--) {
    unsigned long long product =
        ((unsigned long long)decimal->bits[i]) * 10 + carry;
    decimal->bits[i] = (int)(product & 0xFFFFFFFF);
    carry = (unsigned int)(product >> 32);
    carry = ~carry + 1;
  }
  // Handle the highest 32 bits separately to prevent overflow
  unsigned long long product =
      ((unsigned long long)decimal->bits[3] & 0x7FFFFFFF) * 10 + carry;
  decimal->bits[3] =
      ((decimal->bits[3] & 0x80000000) | (int)(product & 0xFFFFFFFF));
  carry = (unsigned int)(product >> 32);
}

// void align_s21_decimals(s21_decimal *a, s21_decimal *b) {
//   int a_scale = (a->bits[3] >> 16) & 0xFF;
//   int b_scale = (b->bits[3] >> 16) & 0xFF;
//   if (a_scale > b_scale) {
//     int diff_scale = a_scale - b_scale;
//     multiply_by_power_of_10(b, diff_scale);
//     b->bits[3] = (b_scale << 16) | (b->bits[3] & 0x80000000);
//   } else if (b_scale > a_scale) {
//     int diff_scale = b_scale - a_scale;
//     multiply_by_power_of_10(a, diff_scale);
//     a->bits[3] = (a_scale << 16) | (a->bits[3] & 0x80000000);
//   }
// }

void __turn_info_into_decimal__(int scale, int sign, s21_decimal *dst) {
  // Set unused bits to zero
  dst->bits[3] &= 0xFF000000;
  // Set exponent bits
  dst->bits[3] |= (scale << 16) & 0x00FF0000;
  // Set sign bit
  dst->bits[3] |= (sign << 31) & 0x80000000;
}

void round_decimal(s21_decimal *decimal) {
  int scale = (decimal->bits[3] >> 16) & 0xFF; // extract the scale factor
  int sign = decimal->bits[3] >> 31;           // extract the sign bit
  int round_bit =
      decimal->bits[2] & (1 << 31); // extract the round bit (the most
                                    // significant bit of the middle 32 bits)

  if (scale == 0) {
    // if the scale factor is 0, the decimal is an integer and rounding is
    // unnecessary
    return;
  }

  // determine the value to add to the decimal to round it
  int round_value = 0;
  if (round_bit != 0) {
    // if the round bit is set, the decimal needs to be rounded up
    round_value = 1;
  }
  // add the round value to the least significant 32 bits of the decimal
  decimal->bits[0] += round_value;

  // check for carry to middle 32 bits
  if (decimal->bits[0] < round_value) {
    decimal->bits[1]++;
    // check for carry to high 32 bits
    if (decimal->bits[1] == 0) {
      decimal->bits[2]++;
    }
  }

  // clear the scale factor and sign bit
  decimal->bits[3] &= 0x00FFFFFF;

  // update the scale factor if necessary
  if (round_value != 0) {
    // if rounding occurred, the scale factor needs to be incremented
    scale++;
  }
  decimal->bits[3] |= scale << 16;

  // restore the sign bit
  decimal->bits[3] |= sign << 31;
}

// s21_decimal add(s21_decimal a, s21_decimal b) {
//   s21_decimal result;
//   int carry = 0;

//   // Iterate over each 32-bit word in the Decimal value
//   for (int i = 2; i > -1; i--) {
//     // Add the corresponding words from a and b, along with any carry from the
//     // previous word
//     long long sum = (long long)a.bits[i] + (long long)b.bits[i] + carry;

//     // Extract the lower 32 bits of the sum and store it in the result
//     result.bits[i] = (int)sum;

//     // Calculate the carry for the next word
//     carry = (int)(sum >> 32);
//   }

//   // Check for overflow
//   if ((a.bits[3] ^ b.bits[3]) & (result.bits[3] ^ a.bits[3])) {
//     // Overflow occurred, so set the result to the maximum value of a Decimal
//     result.bits[0] = result.bits[1] = result.bits[2] = 0xffffffff;
//     result.bits[3] = 0x7f000000;
//   } else {
//     // No overflow, so adjust the scale of the result to match a and b
//     result.bits[3] = a.bits[3];
//   }

//   return result;
// }

#include <stdio.h>


long long add(unsigned long long a, long long b) {
    long long carry;
    while (b != 0) {
        carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

void longlong_to_ints(unsigned long long num, unsigned int *int1, unsigned int *int2) {
    *int1 = (unsigned int)(num >> 32);  // extract upper 32 bits
    *int2 = (unsigned int)(num & 0xffffffff);  // extract lower 32 bits
}

int main() {
  unsigned long long int num = 1234567890123456789;
  unsigned int int1, int2;
  longlong_to_ints(num, &int1, &int2);
  printf("%d %d\n", int1, int2);  // prints -516995667 -1594969947
  return 0;
}


// int main() {

//   s21_decimal p1 = {0, 1, 2147483647, 0};
//   s21_decimal p2 = {0, 5, 2147443647, 0};

//   __turn_info_into_decimal__(1, 0, &p1);
//   __turn_info_into_decimal__(1, 0, &p2);

//   unsigned int a = 4294967295;  // 0xffffffff
//   unsigned int b = 1;
//   unsigned int sum;
//   unsigned int overflow;
//   add_with_overflow(a, b, &sum, &overflow);
//   printf("%u + %u = %u\n", a, b, sum);
//   printf("Overflow: %u\n", overflow);




//   printf("bits[0] = %d\n", p1.bits[0]);
//   printf("bits[1] = %d\n", p1.bits[1]);
//   printf("bits[2] = %d\n", p1.bits[2]);
//   printf("bits[3] = %d\n\n", p1.bits[3]);
//   printf("bits[0] = %d\n", p2.bits[0]);
//   printf("bits[1] = %d\n", p2.bits[1]);
//   printf("bits[2] = %d\n", p2.bits[2]);
//   printf("bits[3] = %d\n", p2.bits[3]);

//   return 0;
// }

// 1111111111111111110110001110111110