// To provide commonly used math routines, algorithms, and constants

#include "math_util.h"

namespace math {

// Function to calculate factorial
float128 factorial(float128 num) {
  if (num == 0.0 || num == 1.0) {
    return 1.0;
  } else {
    return num * factorial(num - 1.0);
  }
}

// Mass to energy calculation
long double EMC2(long double kilograms) {
  long double mass = kilograms;
  long double joules = mass * pow(c, 2);
  long double energy = joules;
  return energy;
}

// φ
long double goldenRatio() {
  long double r = (1 + std::sqrt(5)) / 2;
  return r;
}

} // namespace math

namespace algorithms {

// Simplest approximation, from ancient times
float floatDivPi() {
  float dividend = 355.0f;
  float divisor = 113.0f;
  float pi_float = dividend / divisor;

  std::wcout << std::setprecision(MAX_LOADSTRING) << __FUNC__
             << "() returned " << pi_float << WNL << ENDL;

  return pi_float;
}

} // namespace algorithms

namespace util {

void util::DumpMathConstants(bool do_dump) {
  if (do_dump) {
    std::wostringstream dump;
    dump << std::setprecision(MAX_LOADSTRING);
    dump << "M_E: " << M_E << NL;
    dump << "M_LOG2E: " << M_LOG2E << NL;
    dump << "M_LOG10E: " << M_LOG10E << NL;
    dump << "M_LN2: " << M_LN2 << NL;
    dump << "M_LN10: " << M_LN10 << NL;
    dump << "M_PI: " << M_PI << NL;
    dump << "M_PI_2: " << M_PI_2 << NL;
    dump << "M_PI_4: " << M_PI_4 << NL;
    dump << "M_1_PI: " << M_1_PI << NL;
    dump << "M_2_PI: " << M_2_PI << NL;
    dump << "M_2_SQRTPI: " << M_2_SQRTPI << NL;
    dump << "M_SQRT2: " << M_SQRT2 << NL;
    dump << "M_SQRT1_2: " << M_SQRT1_2 << NL;
    std::wcout << dump.str() << ENDL;

    std::wcout << "UINT_MAX: " << UINT_MAX << ENDL;
    std::wcout << "ULONG_MAX: " << ULONG_MAX << ENDL;
    std::wcout << "ULLONG_MAX: " << ULLONG_MAX << ENDL;
    std::wcout << "SHRT_MAX: " << SHRT_MAX << ENDL;
    std::wcout << "INT_MAX: " << INT_MAX << ENDL;
    std::wcout << "LONG_MAX: " << LONG_MAX << ENDL;
    std::wcout << "LLONG_MAX: " << LLONG_MAX << ENDL;
  } else {
    NULLOPT;
  }
}

} // namespace util
