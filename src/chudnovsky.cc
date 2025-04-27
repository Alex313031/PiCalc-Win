#include <iomanip> 

#include "chudnovsky.h"

// Function to calculate factorial
float128 algorithms::factorial(float128 num) {
  if (num == 0.0 || num == 1.0) {
    return 1.0;
  } else {
    return num * factorial(num - 1.0);
  }
}

// Chudnovsky Algorithm for π calculation
float128 algorithms::chudnovsky(int iterations) {
  float128 pi = zero;
  for (int k = 0; k < iterations; ++k) {
    float128 term =
        (pow(-1.0, k) * factorial(6.0 * k) * (13591409.0 + 545140134.0 * k)) /
        (factorial(3.0 * k) * pow(factorial(k), 3.0) * pow(640320.0, 3.0 * k + 1.5));
    pi += term;
  }
  pi = 1 / (12 * pi);

  std::wcout << std::setprecision(MAX_LOADSTRING) << __FUNCSIG__ << " returned " << pi << std::endl;

  if (is_debug) {
    if (pi == zero || pi != chudnovsky_pi) {
      NOTREACHED();
    }
  }

  return pi;
}
