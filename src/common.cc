// Includes functions that should be generally useful and
// should be useable from anywhere.

#pragma warning(disable : 4996)

#include <chrono>

#include "log.h"
#include "version.h"

#include "common.h"

std::wstring common::StringToWstring(std::string in_string) {
  const std::wstring out_wstring = 
      std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(in_string);
  return out_wstring;
}

std::string common::WStringToString(std::wstring in_wstring) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convertA;

  // use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
  std::string out_string = convertA.to_bytes(in_wstring);
  return out_string;
}

LPCWSTR common::ConvertWstringToLPCWSTR(std::wstring in_wstring) {
  // Allocate memory for the wide string, including null terminator
  wchar_t* out_string = new wchar_t[in_wstring.length() + 1];
  wcscpy_s(out_string, in_wstring.length() + 1, in_wstring.c_str());

  return out_string;
}

LPCWSTR common::ConvertStringToLPCWSTR(std::string in_string) {
  // Use a std::wstring to store the wide character string
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convertW;
  std::wstring new_wstring = convertW.from_bytes(in_string);

  // Allocate memory for the wide string, including null terminator
  wchar_t* wcstrcpy_buffer = new wchar_t[new_wstring.length() + 1];
  wcscpy_s(wcstrcpy_buffer, new_wstring.length() + 1, new_wstring.c_str());

  wchar_t* out_string = wcstrcpy_buffer;

  if (is_dcheck) {
    std::wcerr << std::setprecision(MAX_LOADSTRING) << __FUNCSIG__ << " returned out_string " << out_string << ENDL;
  }

  // Return the pointer to the buffer as LPCWSTR
  return out_string;
}

std::wstring common::ConvertDoubleToWstring(float128 to_convert) {
  std::wostringstream holding_buffer;
  // Clamp to MAX_DECIMAL_PLACES
  holding_buffer << std::setprecision(MAX_DECIMAL_PLACES) << to_convert;
  // swprintf(out_wstring, MAX_DECIMAL_PLACES, L"%9.7lf", to_convert);
  std::wstring out_wstring = holding_buffer.str();

  if (is_dcheck) {
    std::wcerr << std::setprecision(MAX_LOADSTRING) << __FUNCSIG__ << " returned out_wstring " << out_wstring << ENDL;
  }

  return out_wstring;
}

std::string common::GetEnvVariable(const char* env_var_name) {
  // Use C stdlib to get variable
  //const char* env_var_value = getenv(env_var_name);
  
  // Get env variable with CRT lib
  char* env_var_value = nullptr;
  size_t bufferSize = 0;
  std::string return_string;
  errno_t result = _dupenv_s(&env_var_value, &bufferSize, env_var_name);
  // Check if the environment variable was found
  if (result == SUCCESS || env_var_value != NULL || env_var_value != nullptr) {
    std::wcout << __FUNC__ << "() "
               << env_var_name << " = " << env_var_value << ENDL;
    return_string = env_var_value;
  } else {
    std::wcerr << "Environment variable " << env_var_name << " not found." << ENDL;
    return_string = "NULL";
  }
  
  return return_string;
}

namespace common {
std::wstring GetVersionString() {
  std::wstringstream VerStream;
  VerStream << MAJOR_VERSION
            << "."
            << MINOR_VERSION
            << "."
            << BUILD_VERSION;
  std::wstring kVerString = VerStream.str();
  if (is_dcheck) {
    std::wcout << __FUNC__ << "() returned "
               << VERSION_STRING << ENDL;
  }
  return kVerString;
}
}

int common::MakeInfoMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents) {
  // Play sound
  MessageBeep(MB_ICONINFORMATION);

  // Make messagebox
  LPCWSTR kInfoTitle = common::ConvertStringToLPCWSTR(msbox_title);
  LPCWSTR kInfoMessage = common::ConvertStringToLPCWSTR(msbox_contents);
  int infoMsgBox = MessageBoxExW(
    hWnd,
    kInfoMessage,
    kInfoTitle,
    MB_SETFOREGROUND | MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON1,
    0
  );

  switch (infoMsgBox) {
    case IDOK:
      std::wcout << "Pressed OK in " << __FUNC__ << ENDL;
      break;
    default:
      break;
  }

  return infoMsgBox;
}

int common::MakeWarnMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents) {
  MessageBeep(MB_ICONWARNING);

  LPCWSTR kWarnTitle = common::ConvertStringToLPCWSTR(msbox_title);
  LPCWSTR kWarnMessage = common::ConvertStringToLPCWSTR(msbox_contents);
  int warnMsgBox = MessageBoxExW(
    hWnd,
    kWarnMessage,
    kWarnTitle,
    MB_SETFOREGROUND | MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2,
    0
  );

  switch (warnMsgBox) {
    case IDOK:
      std::wcout << "Pressed OK in " << __FUNC__ << ENDL;
      break;
    case IDCANCEL:
      std::wcout << "Pressed Cancel in " << __FUNC__ << ENDL;
      break;
    default:
      break;
  }

  return warnMsgBox;
}

int common::MakeErrorMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents) {
  MessageBeep(MB_ICONERROR);

  LPCWSTR kErrorTitle = common::ConvertStringToLPCWSTR(msbox_title);
  std::wstring wstr =
      common::StringToWstring(msbox_contents) + L"\n" + L"(Press Yes to crash with debug log)";
  LPCWSTR kErrorMessage = common::ConvertWstringToLPCWSTR(wstr);
  int errMsgBox = MessageBoxExW(
    hWnd,
    kErrorMessage,
    kErrorTitle,
    MB_SETFOREGROUND | MB_ICONERROR | MB_YESNOCANCEL | MB_DEFBUTTON2,
    0
  );

  switch (errMsgBox) {
    case IDYES:
      std::wcout << "Pressed Yes in " << __FUNC__ << ENDL;
      common::ExecuteNoNo(FALSE);
      break;
    case IDNO:
      std::wcout << "Pressed No in " << __FUNC__ << ENDL;
      break;
    case IDCANCEL:
      std::wcout << "Pressed Cancel in " << __FUNC__ << ENDL;
      break;
    default:
      break;
  }

  return errMsgBox;
}


// Intentionally execute an illegal instruction to kill the program
// See http://ref.x86asm.net/coder32.html
void AsmIllegalInstr() {
#if defined(_WIN32) && !defined(_WIN64)
  // 32 bit assembly code
  __asm int 3 // Execute int3 interrupt
  __asm {
          UD2 // Execute 0x0F, 0x0B
        }
#elif defined(_WIN64) // x86_64
  // MSVC-specific ud2
  __debugbreak();
#else // ARM64
  std::wcout << __FUNC__ << " not implemented for this architecure";
  __debugbreak();
#endif // _WIN32
}

// Either kill program or halt and wait for debugger
void common::ExecuteNoNo(bool trap) {
  if (trap) {
    base::LOG(ERROR, "Halting execution now!");
    AsmIllegalInstr();
  } else {
    // Win32 only
    base::LOG(ERROR, "Pausing execution for debugging...");
    system("pause");
  }
}

// Dumb equivalent of Chromium's implementation
void NotReachedImpl(std::string func_name) {
  // Whether we are fatal or not
  static constexpr bool notreached_trap = is_dcheck || test_trap;
  std::wstring func_string = common::StringToWstring(func_name);
  std::wcout << "NOTREACHED(): " << func_string << ENDL;
  common::ExecuteNoNo(notreached_trap);
}

std::string base::GetTimestampA() {
  auto time_now = std::chrono::system_clock::now();
  std::time_t in_time_t = std::chrono::system_clock::to_time_t(time_now);

  std::stringstream chronos_string;
  chronos_string << std::put_time(std::localtime(&in_time_t),
                                                 /* "%Y-%m-%d %H:%M:%S"); */
                                                 "%T");

  return chronos_string.str();
}

std::wstring base::GetTimestampW() {
  std::string chronos_string = GetTimestampA();
  std::wstring chronos_wstring =
      common::StringToWstring(chronos_string);

  return chronos_wstring;
}
