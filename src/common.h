#ifndef COMMON_H_
#define COMMON_H_

#pragma once

#include "framework.h"

#ifndef MAX_LOADSTRING
 #define MAX_LOADSTRING 1000
#endif // MAX_LOADSTRING

#ifndef MAX_DECIMAL_PLACES
 #define MAX_DECIMAL_PLACES 1024 // Cap at 1024
#endif // MAX_DECIMAL_PLACES

#ifdef _DCHECK
 #define DCHECK 1
#else
 #define DCHECK 0
#endif // _DCHECK

constexpr bool is_dcheck = DCHECK && DCHECK == 1;

#if defined DEBUG || defined _DEBUG
 constexpr bool is_debug = true;
#else
 constexpr bool is_debug = false;
#endif // defined DEBUG || defined DEBUG

constexpr LONG32 kKibi = MAX_DECIMAL_PLACES;

constexpr wchar_t kDebugPiString[] = L"DebugPiString[] Pi WCHAR Test";

#endif // COMMON_H_
