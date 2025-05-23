#pragma once

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <limits>
#include <intrin.h>

// C++17 or higher
#if __cplusplus >= 201703L
 // To be able to use <codecvts>
 #define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
 #include <optional>// Useful like nullopt below
#endif

#ifndef NULLOPT
#if __cplusplus >= 201703L
 #define NULLOPT std::nullopt
#else
 #define NULLOPT nullptr
#endif // __cplusplus >= 201703L
#endif // NULLOPT

#ifndef __FUNC__
 #define __FUNC__ __func__
#endif // __FUNC__

// Maximum amount of places we can calculate to
#ifndef MAX_DECIMAL_PLACES
 #define MAX_DECIMAL_PLACES 1024 // Cap at 1024 for sanity
#endif // MAX_DECIMAL_PLACES

// Debug CHECKs
#ifdef _DCHECK
 #define DCHECK 1
#else
 #define DCHECK 0
#endif // _DCHECK

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

constexpr long double c = 299792458.0; // Speed of light in m/s

#endif // CONSTANTS_H_
