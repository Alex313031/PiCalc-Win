#pragma once

#ifndef LOG_H_
#define LOG_H_

#include "common.h"

constexpr int INFO = 0;
constexpr int WARN = 1;
#ifdef ERROR
#undef ERROR
 constexpr int ERROR = 2;
#endif
constexpr int MAX_LOG = 3;

void LOG(int log_level);

#endif // LOG_H_
