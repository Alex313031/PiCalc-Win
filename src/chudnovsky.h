#pragma once

#ifndef CHUDNOVSKY_H_
#define CHUDNOVSKY_H_

#include <cwchar>

#include "common.h"

namespace {
  // Maximum times to run algorithm for
  static constexpr int iterations = 28;

  static constexpr float128 chudnovsky_pi =
      3.141592653589793560087173318606801331043243408203125;
}

namespace algorithms {
  // Chudnovsky Algorithm for Pi calculation
  float128 chudnovsky(int iterations);
  // Simple factorial function
  float128 factorial(float128 num);
};

#endif // CHUDNOVSKY_H_
