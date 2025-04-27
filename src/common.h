#pragma once

#ifndef COMMON_H_
#define COMMON_H_

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#if __cplusplus >= 201703L
 // C++17 or higher
 #include <optional>
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

#ifdef __cpp_lib_string_view
  // std::string_view is supported
  #include <string_view>
#endif

#include "framework.h"

// Maximum .res loadstring length
#ifndef MAX_LOADSTRING
 #define MAX_LOADSTRING MAX_PATH // Traditionally 255
#endif // MAX_LOADSTRING

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
#ifndef float128
 #define float128 long double
#endif

// Exit codes
#ifndef SUCCESS_CODE
 #define SUCCESS_CODE 0
#endif
#ifndef ERROR_CODE
 #define ERROR_CODE 1
#endif

constexpr int SUCCESS = SUCCESS_CODE;
constexpr int FAILED = ERROR_CODE;

constexpr float128 zero = 0.000;

// Bool to be used externally to test if DCHECK is on
constexpr bool is_dcheck = DCHECK && DCHECK == 1;

// For testing the different trap functions
const bool test_trap = false;

// Same as above but for DEBUG/NDEBUG
#if defined DEBUG || defined _DEBUG
 constexpr bool is_debug = true;
#else
 constexpr bool is_debug = false;
#endif // defined DEBUG || defined DEBUG

// Alias for default window positioning
constexpr int kUseDefault = CW_USEDEFAULT;

// Functions every module should be able to access
namespace common {
  // 32 bit long pointer for 2^10
  constexpr LONG32 kKibi = MAX_DECIMAL_PLACES;

  // Get Windows version info
  std::string const GetOSVersion();

  // Convert std::string to std::wstring
  std::wstring StringToWstring(std::string in_string);

  // Convert std::wstring to std::string
  std::string WStringToString(std::wstring in_wstring);

  // Convert a std::string to LPCWSTR
  LPCWSTR ConvertStringToLPCWSTR(std::string in_string);

  // Convert a std::wstring to LPCWSTR
  LPCWSTR ConvertWstringToLPCWSTR(std::wstring in_wstring);

  // Convert long double to wide character string
  std::wstring ConvertDoubleToWstring(float128 to_convert);

  std::string GetEnvVariable(const char* env_var_name);

  // Message box types
  int MakeInfoMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents);

  int MakeWarnMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents);

  int MakeErrorMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents);

  // Kill or set debug breakpoint
  void ExecuteNoNo(bool trap);
}

// WINDOWEX styles aliases, not an exhaustive list
namespace WinStyle {
  constexpr DWORD kTiled = WS_TILEDWINDOW;
  constexpr DWORD kBordered = WS_BORDER;
  constexpr DWORD kChild = WS_CHILDWINDOW;
  constexpr DWORD kDialog = WS_DLGFRAME;
  constexpr DWORD kThickFrame = WS_THICKFRAME;
  constexpr DWORD kSizeAble = WS_SIZEBOX;
  constexpr DWORD kSysMenu = WS_SYSMENU;
  constexpr DWORD kVScroll = WS_VSCROLL;
}

namespace TextStyle {
  // Color constants.
  constexpr COLORREF rgbRed    = 0x000000FF;
  constexpr COLORREF rgbGreen  = 0x0000FF00;
  constexpr COLORREF rgbYellow = 0x0000FFFF;
  constexpr COLORREF rgbBlue   = 0x00FF0000;
  constexpr COLORREF rgbBlack  = 0x00000000;
  constexpr COLORREF rgbWhite  = 0x00FFFFFF;
}

static int NT_MAJOR = 0;
static int NT_MINOR = 0;
static int NT_BUILD = 0;

// Execute an assembly instruction to halt execution
inline void AsmIllegalInstr();

inline void NOTREACHED();

#endif // COMMON_H_
