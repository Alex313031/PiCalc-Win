#pragma once

#ifndef ALGORITHMS_CHUDNOVSKY_H_
#define ALGORITHMS_CHUDNOVSKY_H_

#include <cwchar>

#include "math_util.h"

namespace {
  // Maximum iterations to run chudnovsky algorithm for
  static constexpr int max_iterations = 28;

  // For checking calculation of chud
  static constexpr float128 chudnovsky_pi =
      3.141592653589793560087173318606801331043243408203125;
};

namespace algorithms {
  // Chudnovsky algorithm for Pi calculation
  float128 chudnovsky(int iterations);
};

#endif // ALGORITHMS_CHUDNOVSKY_H_
