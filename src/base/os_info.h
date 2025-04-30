#pragma once

#ifndef BASE_OS_INFO_H
#define BASE_OS_INFO_H

#include "../common.h"

namespace {
  static ULONG NT_MAJOR = 0;

  static ULONG NT_MINOR = 0;

  static ULONG NT_BUILD = 0;

  static std::wstring NT_CSD_VERSION;

  static std::string NT_SERVICE_PACK;

  static USHORT NT_SUITE;

  static UCHAR NT_TYPE;

  static std::string NT_FEATURE_VERSION;

  static std::string NT_POST_STRING;
}

static bool is_win11;

namespace base {
  // Get Windows version info
  std::string const GetOSName();
  std::string const GetNTString();
}

#endif //BASE_OS_INFO_H
