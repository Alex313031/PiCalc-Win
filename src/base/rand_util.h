#pragma once

#ifndef BASE_RAND_UTIL_H
#define BASE_RAND_UTIL_H

#include "../framework.h"

// A non-overflowing average function
#define AVG_2S_COMPLEMENT(x, y) ((x) & (y)) + (((x) ^ (y)) / 2)

// Default dRandManual() iterations count
static int dRandIterationsCount = 0;

// Whether to always use dRandManual(), no matter the compiler
static const bool force_regular_drand = is_debug;

/*  See: https://www.azillionmonkeys.com/qed/random.html

    Randutil is meant for creating very accurate pseudo random numbers. The
  problem with the typical methods is that they are either approximations
  or limited to small ranges. The functions given below use double
  precision floating point for all internal calculations without introducing
  any runaway accuracy problems. Hence all output should considered to be
  accurate within a few ulps (you can assume a handful of round off errors.)

  Where size_t is used, only values which are round trip convertable through
  a double precision floating point number are valid. On most systems this
  is either 32 or 53 bits worth.


    int RandBiased(double x);

    Returns 1 with a probability of x otherwise a 0.  It is assumed that
    x is between 0.0 and 1.0 inclusive, otherwise x will behave as if
    clamped to that range.


    size_t RandSlot(const double slots[n-1], size_t n);

    Picks an index from 0 to n-1 inclusive, where the array slots[] is
    at least n-1 elements, and each entry must be a monotonically non-
    decreasing sequence of probabilities. If the slots array does not
    follow the conditions required, this function may be very slow and
    will output meaningless results.


    size_t RandRange(size_t n);

    Picks a value from 0 to n-1 inclusive with equal expected
    likelihood.

    This is commonly implemented as (PRNG_FUNCTION() % n) which is
    inescapably inaccurate if n does not divide evenly into
    (1+PRNG_MAXIMUM) let alone if n is larger than 1+PRNG_MAXIMUM.

    TODO: switch to iterated method if n is too large for double
    precision accuracy.


    long double dRand();
    Picks a uniformly value between 0.0 and 1.0 .


    unsigned int RandReSeed(void);

    Reseed the random number generator and return the seed used.

  TODO: Generalize by allowing for other sources of entropy via callbacks
  and try to comply with the Fortuna mechanism.

*/

// Static entropy state structure.
static struct {
  int which;

  union {
    time_t ent;
    unsigned int seed;
  } timet;

  union {
    clock_t ent;
    unsigned int seed;
  } clockt;

  union {
    unsigned int ent;
    unsigned int seed;
  } counter;
} entropy = {0};

#ifdef __cplusplus
namespace randutil {
#endif

#ifdef __cplusplus
 extern "C" {
#endif

 // Returns 1 or 0, biasing more towards 1 with higher bias double value
 extern int RandBiased(double bias);

 // For arrays
 extern size_t RandSlot(const double slots[/* n-1 */], size_t n);

 // Random Range Functions
 extern size_t RandRange(size_t n);
 extern float fRandRange(float in_range);
 extern long double dRandRange(double in_range);
 extern int iRandRange(int in_range);

 // Must be run before anytime one of the functions below are called
 extern unsigned int RandReSeed(void);

 // Pick best number of iterations for dRandManual()
 static int dRandIterationsRequired();

 // drand functions
 static long double dRandGCC();
 static long double dRandMSVC();
 static long double dRandManual();

 extern long double dRand();

#ifdef __cplusplus
 }
#endif

 void LogRand();
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // BASE_RAND_UTIL_H
