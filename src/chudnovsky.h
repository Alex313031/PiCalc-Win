#pragma once

#ifndef CHUDNOVSKY_H_
#define CHUDNOVSKY_H_

#include <cwchar>

#include "common.h"

namespace {
  // Maximum iterations to run chudnovsky algorithm for
  static constexpr int max_iterations = 28;

  // Default number of circle test trials for Monte Carlo method
  static constexpr int test_trials = 1000000;

  // 32 Bit RAND_MAX
  static constexpr int rand_max32 = 32767;

  // 64 bits x 32 Bit RAND_MAX
  static constexpr int max_trials = rand_max32 * 64; // 2097088

  // For checking calculation of chud
  static constexpr float128 chudnovsky_pi =
      3.141592653589793560087173318606801331043243408203125;
}

namespace algorithms {
  // Simple factorial function
  float128 factorial(float128 num);

  // Chudnovsky algorithm for Pi calculation
  float128 chudnovsky(int iterations);

  // Monte Carlo algorithm for Pi calculation
  float128 montecarlo(int trials);

  // Simple 355/113 approximation
  float floatDivPi();
};

#endif // CHUDNOVSKY_H_
