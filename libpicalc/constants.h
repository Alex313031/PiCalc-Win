#ifndef LIBPICALC_CONSTANTS_H_
#define LIBPICALC_CONSTANTS_H_

#include "compiler_specific.h"

// Define float128 differently depending on compiler
#ifndef __float128
 typedef long double float128;
#else
 typedef __float128 float128;
#endif // __float128

// Exit codes
#ifndef SUCCESS_CODE
 #define SUCCESS_CODE 0
#endif
#ifndef ERROR_CODE
 #define ERROR_CODE 1
#endif

// Easier line endings
#ifndef ENDL
 #define ENDL std::endl
#endif // ENDL
#ifndef NL
 // Newline
 #define NL "\n"
 // "wide" newline
 #define WNL L"\n"
#endif // ENDL

// Maximum .res loadstring length
#ifndef MAX_LOADSTRING
 #define MAX_LOADSTRING MAX_PATH // Traditionally 255
#endif // MAX_LOADSTRING

#if !defined(SUCCESS) && !defined(FAILED)
 static constexpr int SUCCESS = SUCCESS_CODE;
 static constexpr int FAILED = ERROR_CODE;
#endif // !defined(SUCCESS) || !defined(FAILED)

constexpr float128 fzero = 0.0;

// For testing the different trap functions
const bool test_trap = false;

constexpr long double c = 299792458.0; // Speed of light in m/s

#endif // LIBPICALC_CONSTANTS_H_
