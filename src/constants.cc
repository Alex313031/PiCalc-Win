#include "common.h"

#include "constants.h"

// Mass to energy calculation
long double math::EMC2(long double kilograms) {
  long double mass = kilograms;
  long double joules = mass * pow(c, 2);
  long double energy = joules;
  return energy;
}

// φ
long double math::goldenRatio() {
  long double r = (1 + std::sqrt(5)) / 2;
  return r;
}
