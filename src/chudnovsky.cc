#include <iomanip> 
#include <random>

#include "chudnovsky.h"

// Function to calculate factorial
float128 algorithms::factorial(float128 num) {
  if (num == 0.0 || num == 1.0) {
    return 1.0;
  } else {
    return num * factorial(num - 1.0);
  }
}

// Chudnovsky Algorithm for π calculation
float128 algorithms::chudnovsky(int iterations) {
  float128 pi = zero;
  for (int k = 0; k < iterations; ++k) {
    float128 term =
        (pow(-1.0, k) * factorial(6.0 * k) * (13591409.0 + 545140134.0 * k)) /
        (factorial(3.0 * k) * pow(factorial(k), 3.0) * pow(640320.0, 3.0 * k + 1.5));
    pi += term;
  }
  pi = 1 / (12 * pi);

  std::wcout << std::setprecision(MAX_LOADSTRING) << WNL << __FUNC__
             << "() returned " << pi << NL;

  if (is_debug) {
    if (pi == zero || pi != chudnovsky_pi) {
      NOTREACHED();
    }
  }

  return pi;
}

float128 algorithms::montecarlo(int trials) {
  int n_trials;
  int n_inside = 0;
  long double circle = 1.0;
  if (trials < 1 || trials >= max_trials || max_trials != 2097088) {
    std::wcerr << __FUNC__ << "() trials too high, setting to 1000" << ENDL;
    n_trials = 1000;
  } else {
    n_trials = trials;
  }

  std::random_device rand_device;
  uint_fast32_t randint = rand_device();
  std::mt19937 mt_rand;
  mt_rand.seed(randint);
  uint_fast32_t gen = mt_rand();
  std::uniform_real_distribution <> dist(0.0, 1.0);

  for (int i = 0; i < n_trials; ++i) {
    long double x = dist(mt_rand);
    long double y = dist(mt_rand);
    if (std::sqrt(x * x + y * y) <= circle) {
      n_inside++;
    }
  }

  long double pi_approx = 4.0 * n_inside / n_trials;

  std::wcout << std::setprecision(MAX_LOADSTRING) << WNL << __FUNC__
             << "() returned " << pi_approx << NL;

  return pi_approx;
}

float algorithms::floatDivPi() {
  float dividend = 355.0f;
  float divisor = 113.0f;
  float pi_float = dividend / divisor;

  std::wcout << std::setprecision(MAX_LOADSTRING) << WNL << __FUNC__
             << "() returned " << pi_float << NL;

  return pi_float;
}
