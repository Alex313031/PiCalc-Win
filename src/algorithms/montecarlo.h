#pragma once

#ifndef ALGORITHMS_MONTECARLO_H_
#define ALGORITHMS_MONTECARLO_H_

#include <cwchar>

#include "math_util.h"

namespace {
  // Default number of circle test trials for Monte Carlo method
  static constexpr int test_trials = 1000000;

  // 32 Bit RAND_MAX
  static constexpr int rand_max32 = 32767;

  // 64 bits x 32 Bit RAND_MAX
  static constexpr int max_trials = rand_max32 * 64; // 2097088
};

namespace algorithms {
  // Monte Carlo algorithm for Pi calculation
  float128 montecarlo(int trials);
};

#endif // ALGORITHMS_MONTECARLO_H_
