#pragma once

#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <sstream>
#include <string>

#include "framework.h"
#include "resource.h"

constexpr int STATUS_GOOD = 0;
constexpr int STATUS_BAD = 1;

// Functions to get main and child window handles from other code
extern HWND getMainHwnd();
extern HWND getChildHwnd();

std::wstring getVersionString();

#endif // COMMON_H_
