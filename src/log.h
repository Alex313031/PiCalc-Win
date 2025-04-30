#pragma once

#ifndef LOG_H_
#define LOG_H_

#include <ctime>

#include "common.h"

enum Verbosity {
  INFO,
  WARN,
  ERR,
  MAX_LOG
};

static Verbosity LogLevel;

static std::string GetTimestamp();

namespace base {
  void LOG(int log_level, std::string log_input);
}

namespace common {
  void LogCompilerInfo(bool do_log);
}

#endif // LOG_H_
