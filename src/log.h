#pragma once

#ifndef LOG_H_
#define LOG_H_

#include "common.h"

enum Verbosity {
  INFO,
  WARN,
  ERR,
  MAX_LOG
};

static Verbosity LogLevel;

namespace base {
  // Formatted log timestamp header
  extern std::wstring GetLogTimeStamp();
  // Baseline, printf style logger
  void LOG(int log_level, std::string log_input);
}

namespace common {
  void LogCompilerInfo(bool do_log);
}

#endif // LOG_H_
