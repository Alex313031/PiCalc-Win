#include <iomanip>
#include <random>

#include "montecarlo.h"

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

  std::wcout << std::setprecision(MAX_LOADSTRING) << __FUNC__
             << "() returned " << pi_approx << WNL << ENDL;

  return pi_approx;
}
