
#include "chudnovsky.h"

// Function to calculate factorial
long double factorial(long double num) {
  if (num == 0.0 || num == 1.0) {
    return 1.0;
  } else {
    return num * factorial(num - 1.0);
  }
}

// Chudnovsky Algorithm for Pi calculation
long double chudnovsky(int iterations) {
  long double pi = 0.0;
  for (int k = 0; k < iterations; ++k) {
    long double term =
        (pow(-1.0, k) * factorial(6.0 * k) * (13591409.0 + 545140134.0 * k)) /
        (factorial(3.0 * k) * pow(factorial(k), 3.0) * pow(640320.0, 3.0 * k + 1.5));
    pi += term;
  }
  pi = 1 / (12 * pi);
  return pi;
}

void convertDoubleToWString() {
  long double to_convert = chudnovsky(iterations);
  wchar_t wchar_array[1024];
  swprintf(wchar_array, 1024, L"%f", to_convert);
}
