#include "libpicalc.h"
#include "libpicalc_dll.h"

#include <iostream>
#include <iomanip>
#include <sstream>

float128 oldfactorial(float128 num) {
  if (num == 0.0 || num == 1.0) {
    return 1.0;
  } else {
    return num * oldfactorial(num - 1.0);
  }
}

// Chudnovsky Algorithm for π calculation
float128 oldchudnovsky(int iterations) {
  float128 pi = fzero;
  for (int k = 0; k < iterations; ++k) {
    float128 term =
        (pow(-1.0, k) * oldfactorial(6.0 * k) * (13591409.0 + 545140134.0 * k)) /
        (oldfactorial(3.0 * k) * pow(oldfactorial(k), 3.0) * pow(640320.0, 3.0 * k + 1.5));
    pi += term;
  }
  pi = 1 / (12 * pi);

  std::wcout << std::setprecision(MAX_LOADSTRING) << __FUNC__
             << "() returned " << pi << WNL << ENDL;

  if (is_debug) {
    if (pi == fzero || pi != old_chudnovsky_pi) {
      //NOTREACHED();
    }
  }

  return pi;
}

mpf_class factorial(int n) {
  mpf_class result = 1.0;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

bool compute_pi_chudnovsky(mpf_class &pi) {
  bool success = false;
  mpf_set_default_prec(GMP_PRECISION);

  const mpf_class C = 426880.0 * sqrt(mpf_class(10005.0));
  mpf_class sum = 0.0;

  const int terms = DIGITS / 14 + 2;

  for (int k = 0; k < terms; ++k) {
    mpf_class num, den, term;

    // Numerator: (6k)! * (13591409 + 545140134k)
    mpf_class a = factorial(6 * k);
    mpf_class b = 13591409.0 + 545140134.0 * k;
    num = a * b;

      // Denominator: (3k)! * (k!)^3 * 640320^(3k)
      mpf_class c = factorial(3 * k);
      mpf_class d = factorial(k);
      d = d * d * d;
      mpf_class e;
      mpf_pow_ui(e.get_mpf_t(), mpf_class(640320.0).get_mpf_t(), 3 * k);

      den = c * d * e;

      term = num / den;
      if (k % 2 != 0) {
        term = -term;
      }

      sum += term;
  }
  mpf_class pi_result = 0.0;
  pi_result = C / sum;

  if (!pi_result || pi_result == 0.0) {
    success = false;
  } else {
    pi = pi_result;
    success = true;
  }

  return success;
}

mpf_class mpf_pi_chudnovsky() {
  mpf_set_default_prec(GMP_PRECISION);

  const mpf_class C = 426880.0 * sqrt(mpf_class(10005.0));
  mpf_class sum = 0.0;

  const int terms = DIGITS / 14 + 2;

  for (int k = 0; k < terms; ++k) {
    mpf_class num, den, term;

    // Numerator: (6k)! * (13591409 + 545140134k)
    mpf_class a = factorial(6 * k);
    mpf_class b = 13591409.0 + 545140134.0 * k;
    num = a * b;

      // Denominator: (3k)! * (k!)^3 * 640320^(3k)
      mpf_class c = factorial(3 * k);
      mpf_class d = factorial(k);
      d = d * d * d;
      mpf_class e;
      mpf_pow_ui(e.get_mpf_t(), mpf_class(640320.0).get_mpf_t(), 3 * k);

      den = c * d * e;

      term = num / den;
      if (k % 2 != 0) {
        term = -term;
      }

      sum += term;
  }
  mpf_class pi_result = 0.0;
  pi_result = C / sum;

  return pi_result;
}

COMPONENT_EXPORT
bool oss_pi_chudnovsky(std::ostringstream &osspi) {
  mpf_set_default_prec(GMP_PRECISION);
  std::ostringstream result;
  mpf_class pi;
  compute_pi_chudnovsky(pi);
  result << std::fixed << std::setprecision(DIGITS) << pi;
  osspi << result.str();
  return true;
}

COMPONENT_EXPORT
bool woss_pi_chudnovsky(std::wostringstream &wosspi) {
  std::ostringstream osspi;
  oss_pi_chudnovsky(osspi);
  std::string pistring = osspi.str();
  std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
  std::wstring piout = wconv.from_bytes(pistring);
  wosspi << piout;
  return true;
}

COMPONENT_EXPORT
bool wpi_chudnovsky(wchar_t &wcpi) {
  std::wostringstream wosspi;
  woss_pi_chudnovsky(wosspi);
  std::wstring wtest = wosspi.str();
  wchar_t *wout = const_cast<wchar_t*>(wtest.c_str());
  wcpi = *wout;
  return true;
}

COMPONENT_EXPORT
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD reason, LPVOID reserved) {
  // Perform actions based on the reason for calling.
  switch(reason) { 
    case DLL_PROCESS_ATTACH:
      // Initialize once for each new process.
      // Return FALSE to fail DLL load.
      break;
    case DLL_THREAD_ATTACH:
      // Do thread-specific initialization.
      break;
    case DLL_THREAD_DETACH:
      // Do thread-specific cleanup.
      break;
    case DLL_PROCESS_DETACH:
      if (reserved != nullptr) {
        break; // do not do cleanup if process termination scenario
      }
      // Perform any necessary cleanup.
      break;
  }
  return TRUE; // Successful DLL_PROCESS_ATTACH.
}
