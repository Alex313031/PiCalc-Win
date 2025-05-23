#pragma once

#ifndef ALGORITHMS_UTIL_H_
#define ALGORITHMS_UTIL_H_

#include "../common.h"

namespace math {
  // Simple factorial function
  float128 factorial(float128 num);

  // Einstein's famous E=mc^2
  long double EMC2(long double kilograms);

  // Calculate the golden ratio directly
  long double goldenRatio();
};

namespace algorithms {
  // Simple 355/113 approximation
  float floatDivPi();
};

namespace util {
  void DumpMathConstants(bool do_dump);
};

#endif // ALGORITHMS_UTIL_H_
