#include <ctime>
#include <iomanip>
#include <iostream>

#include "algorithms/math_util.h"
#include "log.h"

std::wstring base::GetLogTimeStamp() {
  std::wostringstream logstamp;
  logstamp << L"[" << base::GetTimestampW() << L"]";
  return logstamp.str();
}

void base::LOG(int log_level, std::string log_input) {
  if (log_level > MAX_LOG) {
    NOTREACHED();
  }

  std::wostringstream log_buffer;

  LogLevel = static_cast<Verbosity>(log_level);
  std::wstring log_output =
      common::StringToWstring(log_input);

  switch (LogLevel) {
    case INFO:
        log_buffer << GetLogTimeStamp() << " "
                   << log_output << ENDL;
        break;
    case WARN:
        log_buffer << GetLogTimeStamp() << " WARNING: "
                   << log_output << ENDL;
        break;
    case ERR:
        log_buffer << GetLogTimeStamp() << " ERROR: "
                   << log_output << ENDL;
        break;
    case MAX_LOG:
        log_buffer << " ERROR: Don't use MAX_LOG! "
                   << MAX_LOG << ENDL;
        break;
    default:
        NOTREACHED();
  }
  if (log_level == ERR || log_level == MAX_LOG) {
    std::wcerr << log_buffer.str();
  } else {
    std::wcout << log_buffer.str();
  }
}

void common::LogCompilerInfo(bool do_log) {
  if (do_log) {
    // Log some compiler preprocessor constants to know what type of build we have
    std::wstring kBuildType;
    if (is_debug && !is_dcheck) {
      kBuildType = L"DEBUG";
    } else if (!is_debug && !is_dcheck) {
      kBuildType = L"RELEASE";
    } else if (!is_debug && is_dcheck) {
      kBuildType = L"DCHECK RELEASE";
    } else if (is_debug && is_dcheck) {
      kBuildType = L"DCHECK DEBUG";
    } else {
      NOTREACHED();
    }
    std::wcout << "Buildtype: " << kBuildType << ENDL;

    // Log MSVC compilation info
    std::wcout << "_MSC_VER = " << _MSC_VER << ENDL;
    std::wcout << "_MSC_FULL_VER = " << _MSC_FULL_VER << ENDL;

    if (is_dcheck) {
#ifdef __STDCPP_THREADS__ // Whether threading is enabled
    std::wcout << "__STDCPP_THREADS__ = " << __STDCPP_THREADS__ << ENDL;
#else
    std::wcout << "__STDCPP_THREADS__ = " << FALSE << ENDL;
#endif // __STDCPP_THREADS__
#ifdef __SSE2__ // Whether we compiled with /arch:SSE2 (or -msse2)
    std::wcout << "__SSE2__ = " << __SSE2__ << ENDL;
#else
    std::wcout << "__SSE2__ = " << FALSE << ENDL;
#endif // __SSE2__
#ifdef __SSE42__ // Whether we compiled with /arch:SSE4.2 (or -msse4.2)
    std::wcout << "__SSE42__ = " << __SSE42__ << ENDL;
#else
    std::wcout << "__SSE42__ = " << FALSE << ENDL;
#endif // __SSE42__
#ifdef __AVX__ // Whether we compiled with /arch:AVX (or -mavx)
    std::wcout << "__AVX__ = " << __AVX__ << ENDL;
#else
    std::wcout << "__AVX__ = " << FALSE << ENDL;
#endif // __AVX__
#ifdef __AVX2__ // Whether we compiled with /arch:AVX2 (or -mavx2)
    std::wcout << "__AVX2__ = " << __AVX2__ << ENDL;
#else
    std::wcout << "__AVX2__ = " << FALSE << ENDL;
#endif // __AVX2__
#ifdef __AVX512BW__ // Whether we compiled with /arch:AVX512 (or -mavx512bw)
    std::wcout << "__AVX512BW__ = " << __AVX512BW__ << ENDL;
#else
    std::wcout << "__AVX512BW__ = " << FALSE << ENDL;
#endif // __AVX512BW__
#ifdef _CHAR_UNSIGNED // Whether char* is considered unsigned
    std::wcout << "_CHAR_UNSIGNED = " << _CHAR_UNSIGNED << ENDL;
#endif // _CHAR_UNSIGNED
#ifdef __CLR_VER // The Common Language Runtime used to compile the app
    std::wcout << "__CLR_VER = " << __CLR_VER << ENDL;
#endif // __CLR_VER
    std::wcout << L"\n";
    }
    util::DumpMathConstants(is_dcheck);
  } else {
    NULLOPT;
  }
}
