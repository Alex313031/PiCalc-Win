#include "libpicalc.h"

float128 oldfactorial(float128 num) {
  if (num == 0.0 || num == 1.0) {
    return 1.0;
  } else {
    return num * oldfactorial(num - 1.0);
  }
}

// Chudnovsky Algorithm for π calculation
float128 oldchudnovsky(int iterations) {
  float128 pi = fzero;
  for (int k = 0; k < iterations; ++k) {
    float128 term =
        (pow(-1.0, k) * oldfactorial(6.0 * k) * (13591409.0 + 545140134.0 * k)) /
        (oldfactorial(3.0 * k) * pow(oldfactorial(k), 3.0) * pow(640320.0, 3.0 * k + 1.5));
    pi += term;
  }
  pi = 1 / (12 * pi);

  std::wcout << std::setprecision(MAX_LOADSTRING) << __FUNC__
             << "() returned " << pi << WNL << ENDL;

  if (is_debug) {
    if (pi == fzero || pi != old_chudnovsky_pi) {
      //NOTREACHED();
    }
  }

  return pi;
}

COMPONENT_EXPORT
mpf_class algorithms::factorial(int n) {
    mpf_class result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

COMPONENT_EXPORT
void algorithms::compute_pi_chudnovsky(mpf_class &pi) {
    mpf_set_default_prec(GMP_PRECISION);

    const mpf_class C = 426880.0 * sqrt(mpf_class(10005.0));
    mpf_class sum = 0.0;

    const int terms = DIGITS / 14 + 2;

    for (int k = 0; k < terms; ++k) {
        mpf_class num, den, term;

        // Numerator: (6k)! * (13591409 + 545140134k)
        mpf_class a = factorial(6 * k);
        mpf_class b = 13591409.0 + 545140134.0 * k;
        num = a * b;

        // Denominator: (3k)! * (k!)^3 * 640320^(3k)
        mpf_class c = factorial(3 * k);
        mpf_class d = factorial(k);
        d = d * d * d;
        mpf_class e;
        mpf_pow_ui(e.get_mpf_t(), mpf_class(640320.0).get_mpf_t(), 3 * k);

        den = c * d * e;

        term = num / den;
        if (k % 2 != 0) {
          term = -term;
        }

        sum += term;
    }

    pi = C / sum;
}
