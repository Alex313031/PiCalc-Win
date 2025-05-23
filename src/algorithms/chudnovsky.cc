#include <iomanip>

#include "chudnovsky.h"

#define fac math::factorial

// Chudnovsky Algorithm for π calculation
float128 algorithms::chudnovsky(int iterations) {
  float128 pi = zero;
  for (int k = 0; k < iterations; ++k) {
    float128 term =
        (pow(-1.0, k) * fac(6.0 * k) * (13591409.0 + 545140134.0 * k)) /
        (fac(3.0 * k) * pow(fac(k), 3.0) * pow(640320.0, 3.0 * k + 1.5));
    pi += term;
  }
  pi = 1 / (12 * pi);

  std::wcout << std::setprecision(MAX_LOADSTRING) << __FUNC__
             << "() returned " << pi << WNL << ENDL;

  if (is_debug) {
    if (pi == zero || pi != chudnovsky_pi) {
      NOTREACHED();
    }
  }

  return pi;
}
