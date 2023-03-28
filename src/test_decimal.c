#include <stdio.h>

int main() {

  unsigned int dividend = 10;
  unsigned int divisor = 3;
  const float divisor_constant = 100.0;

  unsigned int temp = dividend * divisor_constant;
  unsigned int result = temp / divisor;
  float fractional_result = (float)result / divisor_constant;

  printf("Fractional result: %f\n", fractional_result);
  return 0;
}
