#pragma once

#ifndef LIBPICALC_LIBPICALC_H
#define LIBPICALC_LIBPICALC_H

#include "component_export.h"
#include "constants.h"
#include "../framework.h"
#include "../resource.h"

// Include GMP headers last
#include "gmp/gmp.h"
#include "gmp/gmpxx.h"

// Set desired number of digits of Pi
constexpr int DIGITS = 1024;

// Precision to use for Pi calculation
const int PRECISION = DIGITS * 3.32193 + 32;

// Precision bits for GMP itself
const mp_bitcnt_t GMP_PRECISION = static_cast<mp_bitcnt_t>(PRECISION);

// Typedefs for proper HMODULE type safety
//typedef float128 (*oldfactorial)(float128 num);
//typedef float128 (*oldchudnovsky)(int iterations);

#ifdef COMPONENT_BUILD
 // For type safety when getting address of .dll functions
 //typedef bool (*compute_pi_chudnovsky)(mpf_class &pi);
 //typedef mpf_class (pi_chudnovsky)();
#endif // #ifdef COMPONENT_BUILD

// Empty namespace for old functions
namespace {
  // Maximum iterations oldchudnovsky algorithm can be run for without overflow
  static constexpr int max_iterations = 28;

  // For checking calculation of oldchudnovsky below
  static constexpr float128 old_chudnovsky_pi =
      3.141592653589793560087173318606801331043243408203125;

  // Old long double based factorial function
  //float128 oldfactorial(float128 num);

  // Old long double based Pi calculation from PiCalc 0.x.x
  float128 oldchudnovsky(int iterations);
};

// Multi-Precision factorial
mpf_class factorial(int n);

// Chudnovsky algorithm for Pi calculation
bool compute_pi_chudnovsky(mpf_class &pi);

mpf_class mpf_pi_chudnovsky();

#endif // LIBPICALC_LIBPICALC_H
