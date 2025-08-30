// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is used as a precompiled header for both C and C++ files. So
// any C++ headers must go in the __cplusplus block below.

#if defined(BUILD_PRECOMPILE_H_)
#error "You shouldn't include the precompiled header file more than once."
#endif

#define BUILD_PRECOMPILE_H_

#define _USE_MATH_DEFINES

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <memory.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__cplusplus)

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iosfwd>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#ifdef __cpp_lib_string_view
 // std::string_view is supported
 #include <string_view>
#endif

// C++17 or higher
#if __cplusplus >= 201703L
 // To be able to use <codecvts>
 #ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
  #define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
 #endif
 #include <optional>// Useful like nullopt below
#endif

#include <cwchar>

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

#endif  // __cplusplus
