// Includes functions that should be generally useful and
// should be useable from anywhere.

#include "common.h"

std::string const common::GetOSVersion() {
  // Human readable name
  std::string OsVer;
  // NT version number
  std::string NtVer;
  // For obscure versions or pre NT4 SP6
  std::ostringstream debugStream;

  // Use RtlGetVersion from winnt.h, not wdm.h
  NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
  // https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-osversioninfoexw
  OSVERSIONINFOEXW osInfo;

  // Make sure we get the dll
  HMODULE NtDllModule = GetModuleHandleW(L"ntdll");
  if (NtDllModule) {
    *reinterpret_cast<FARPROC*>(&RtlGetVersion) =
        GetProcAddress(NtDllModule, "RtlGetVersion");
  } else {
    RtlGetVersion = nullptr;
  }

  if (RtlGetVersion != nullptr) {
    osInfo.dwOSVersionInfoSize = sizeof osInfo;
    RtlGetVersion(&osInfo);
    NT_MAJOR = osInfo.dwMajorVersion;
    NT_MINOR = osInfo.dwMinorVersion;
    NT_BUILD = osInfo.dwBuildNumber;
  } else {
    common::MakeErrorMessageBox(NULL, "RtlGetVersion", "RtlGetVersion was nullptr");
  }

  if (NT_MAJOR >=3 && NT_MAJOR <=10) {
    NtVer = " " + std::to_string(NT_MAJOR) + "." + std::to_string(NT_MINOR) + "." + std::to_string(NT_BUILD);
  }

  if (NT_MAJOR == 4) {
    // Known to be buggy on NT 4.0, and needs SP6 to work
    switch (NT_MINOR) {
      case 0:
          OsVer = " (Windows NT 4.0)";
          break;
      default:
          debugStream.str("");
          debugStream.clear();
          debugStream << " Unknown Windows"
                      << NT_MAJOR << NT_MINOR << std::endl;
          OsVer = debugStream.str();
          break;
    }
  } else if (NT_MAJOR == 5) {
    switch (NT_MINOR) {
      case 0:
          OsVer = " (Windows 2000)";
          break;
      case 1:
          OsVer = " (Windows XP)";
          break;
      case 2:
          OsVer = " (Windows Server 2003/XP x64)";
          break;
      default:
          debugStream.str("");
          debugStream.clear();
          debugStream << " Unknown Windows"
                      << NT_MAJOR << NT_MINOR << std::endl;
          OsVer = debugStream.str();
          break;
    }
  } else if (NT_MAJOR == 6) {
    switch (NT_MINOR) {
      case 0:
          OsVer = " (Windows Vista or Server 2008)";
          break;
      case 1:
          OsVer = " (Windows 7 or Server 2008 R2)";
          break;
      case 2:
          OsVer = " (Windows 8 or Server 2012)";
          break;
      case 3:
          OsVer = " (Windows 8.1 or Server 2012 R2)";
          break;
      default:
          debugStream.str("");
          debugStream.clear();
          debugStream << " Unknown Windows"
                      << NT_MAJOR << NT_MINOR << std::endl;
          OsVer = debugStream.str();
          break;
    }
  } else if (NT_MAJOR == 10) {
    switch (NT_MINOR) {
      case 0:
          OsVer = " (Windows 10)";
          break;
      case 1:
          OsVer = " (Windows 11)";
          break;
      default:
          debugStream.str("");
          debugStream.clear();
          debugStream << " Unknown Windows "
                      << NT_MAJOR << NT_MINOR << std::endl;
          OsVer = debugStream.str();
          break;
    }
  } else {
    debugStream.str("");
    debugStream.clear();
    debugStream << " Unknown Windows";
    OsVer = debugStream.str();
    NtVer = " NULL";
    NOTREACHED();
  }

  // String with both separated by a space
  std::string OsNtVer = NtVer + OsVer;
  return OsNtVer;
}

std::wstring common::StringToWstring(std::string in_string) {
  const std::wstring out_wstring = 
      std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(in_string);
  return out_wstring;
}

std::string common::WStringToString(std::wstring in_wstring) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convertA;

  //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
  std::string out_string = convertA.to_bytes( in_wstring );
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
    std::wcerr << std::setprecision(MAX_LOADSTRING) << __FUNCSIG__ << " returned out_string " << out_string << std::endl;
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
    std::wcerr << std::setprecision(MAX_LOADSTRING) << __FUNCSIG__ << " returned out_wstring " << out_wstring << std::endl;
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
               << env_var_name << " = " << env_var_value << std::endl;
    return_string = env_var_value;
  } else {
    std::wcerr << "Environment variable " << env_var_name << " not found." << std::endl;
    return_string = "NULL";
  }
  
  return return_string;
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
      std::wcout << "Pressed OK in " << __FUNC__ << std::endl;
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
      std::wcout << "Pressed OK in " << __FUNC__ << std::endl;
      break;
    case IDCANCEL:
      std::wcout << "Pressed Cancel in " << __FUNC__ << std::endl;
      break;
    default:
      break;
  }

  return warnMsgBox;
}

int common::MakeErrorMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents) {
  MessageBeep(MB_ICONERROR);

  LPCWSTR kErrorTitle = common::ConvertStringToLPCWSTR(msbox_title);
  LPCWSTR kErrorMessage = common::ConvertStringToLPCWSTR(msbox_contents);
  int errMsgBox = MessageBoxExW(
    hWnd,
    kErrorMessage,
    kErrorTitle,
    MB_SETFOREGROUND | MB_ICONERROR | MB_YESNOCANCEL | MB_DEFBUTTON2,
    0
  );

  switch (errMsgBox) {
    case IDYES:
      std::wcout << "Pressed Yes in " << __FUNC__ << std::endl;
      break;
    case IDNO:
      std::wcout << "Pressed No in " << __FUNC__ << std::endl;
      break;
    case IDCANCEL:
      std::wcout << "Pressed Cancel in " << __FUNC__ << std::endl;
      break;
    default:
      break;
  }

  return errMsgBox;
}


// Intentionally execute an illegal instruction to kill the program
// See http://ref.x86asm.net/coder32.html
void AsmIllegalInstr() {
#ifdef _WIN64
  __debugbreak();
#else // 32 bit assembly code
  __asm {
    // Execute 0x0F, 0x0B
    UD2
  }
#endif // defined _WIN64
}

// Either kill program or halt and wait for debugger
void common::ExecuteNoNo(bool trap) {
  if (trap) {
    AsmIllegalInstr();
  } else {
    // Win32 only
    system("pause");
  }
}

// Dumb equivalent of Chromium's implementation
void NOTREACHED() {
  // Whether we are fatal or not
  static constexpr bool notreached_trap = is_dcheck || test_trap;
  std::wcout << "NOTREACHED()!" << std::endl;
  common::ExecuteNoNo(notreached_trap);
}
