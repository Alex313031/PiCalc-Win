#ifndef LIBPICALC_COMPILER_SPECIFIC_H_
#define LIBPICALC_COMPILER_SPECIFIC_H_

// A wrapper around `__has_attribute()`, which is similar to the C++20-standard
// `__has_cpp_attribute()`, but tests for support for `__attribute__(())`s.
// Compilers that do not support this (e.g. MSVC) are also assumed not to
// support `__attribute__`, so this is simply mapped to `0` there.
//
// See also:
//   https://clang.llvm.org/docs/LanguageExtensions.html#has-attribute
#if defined(__has_attribute)
 #define HAS_ATTRIBUTE(x) __has_attribute(x)
#else
 #define HAS_ATTRIBUTE(x) 0
#endif // defined(__has_attribute)

// A wrapper around `__has_builtin`, similar to `HAS_ATTRIBUTE()`.
//
// See also:
//   https://clang.llvm.org/docs/LanguageExtensions.html#has-builtin
#if defined(__has_builtin)
 #define HAS_BUILTIN(x) __has_builtin(x)
#else
 #define HAS_BUILTIN(x) 0
#endif // defined(__has_builtin)

// A wrapper around `__has_feature`, similar to `HAS_ATTRIBUTE()`.
//
// See also:
//   https://clang.llvm.org/docs/LanguageExtensions.html#has-feature-and-has-extension
#if defined(__has_feature)
 #define HAS_FEATURE(FEATURE) __has_feature(FEATURE)
#else
 #define HAS_FEATURE(FEATURE) 0
#endif // defined(__has_feature)

// Annotates a function indicating it should not be inlined.
//
// You may also want `noopt` if your goal is to preserve a function call even
// for the most trivial cases; see
// https://stackoverflow.com/questions/54481855/clang-ignoring-attribute-noinline/54482070#54482070.
//
// See also:
//   https://clang.llvm.org/docs/AttributeReference.html#noinline
//
// Usage:
// ```
//   _NOINLINE void Func() {
//     // This body will not be inlined into callers.
//   }
// ```
#ifndef _NOINLINE
 #if __has_cpp_attribute(clang::noinline) // Clang
  #define _NOINLINE [[clang::noinline]]
 #elif __has_cpp_attribute(gnu::noinline) // GCC
  #define _NOINLINE [[gnu::noinline]]
 #elif __has_cpp_attribute(msvc::noinline) // MSVC
  #define _NOINLINE [[msvc::noinline]]
 #else // Dummy
  #define _NOINLINE
 #endif
#endif // _NOINLINE

// Annotates a function indicating it should always be inlined.
//
// See also:
//   https://clang.llvm.org/docs/AttributeReference.html#always-inline-force-inline
//
// Usage:
// ```
//   _INLINE void Func() {
//     // This body will be inlined into callers whenever possible.
//   }
// ```
//
// Since `_INLINE` is performance-oriented but can hamper debugging,
// ignore it in debug mode.
#if defined(NDEBUG)
 #if __has_cpp_attribute(clang::always_inline) // Clang
  #define _INLINE [[clang::always_inline]] inline
 #elif __has_cpp_attribute(gnu::always_inline // GCC
  #define _INLINE [[gnu::always_inline]] inline
 #elif defined(COMPILER_MSVC) // MSVC
  #define _INLINE __forceinline
 #endif
#endif
// Fallback
#if !defined(_INLINE) // Other compilers
 #define _INLINE inline
#endif

// Evaluates to a string constant containing the function name.
//
// See also:
//   https://learn.microsoft.com/en-us/cpp/cpp/func
//   https://en.cppreference.com/w/c/language/function_definition#func
//
// Usage:
// ```
//   void Func(int arg) {
//     std::cout << __FUNC__;  // Prints `Func` or similar.
//   }
// ```
#ifndef __FUNC__
 #define __FUNC__ __func__
#endif

// Evaluates to a string constant containing the function signature.
//
// See also:
//   https://clang.llvm.org/docs/LanguageExtensions.html#source-location-builtins
//   https://en.cppreference.com/w/c/language/function_definition#func
//
// Usage:
// ```
//   void Func(int arg) {
//     std::cout << PRETTY_FUNCTION;  // Prints `void Func(int)` or similar.
//   }
// ```
#ifndef PRETTY_FUNCTION
 #if defined(COMPILER_GCC)
  #define PRETTY_FUNCTION __PRETTY_FUNCTION__
 #elif defined(COMPILER_MSVC)
  #define PRETTY_FUNCTION __FUNCSIG__
 #else
  #define PRETTY_FUNCTION __func__
 #endif
#endif // PRETTY_FUNCTION

// Define DCHECK indirectly
#if defined(IS_DCHECK)
 #define DCHECK 1
#else
 #define DCHECK 0
#endif

// Internal bool to be used externally to test if DCHECK is on
constexpr bool is_dcheck = DCHECK == 1;

// Internal bool to check debug defines
#if defined(DEBUG) && !defined(NDEBUG)
 constexpr bool is_debug = true;
#else
 constexpr bool is_debug = false;
#endif // defined DEBUG || defined DEBUG

#endif // LIBPICALC_COMPILER_SPECIFIC_H_
