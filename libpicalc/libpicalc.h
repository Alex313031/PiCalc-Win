#pragma once

#ifndef LIBPICALC_LIBPICALC_H
#define LIBPICALC_LIBPICALC_H

#include "component_export.h"

#include "constants.h"
#include "../framework.h"
#include "../resource.h"

namespace {
  // Maximum iterations to run chudnovsky algorithm for
  static constexpr int max_iterations = 28;

  // For checking calculation of chud
  static constexpr float128 chudnovsky_pi =
      3.141592653589793560087173318606801331043243408203125;
};

namespace math {
  COMPONENT_EXPORT float128 factorial(float128 num);
};

namespace algorithms {
  // Chudnovsky algorithm for Pi calculation
  COMPONENT_EXPORT float128 chudnovsky(int iterations);
};

#endif // LIBPICALC_LIBPICALC_H
