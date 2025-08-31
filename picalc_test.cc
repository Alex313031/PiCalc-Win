#include "picalc_main.h"

#ifndef COMPONENT_BUILD
#include "libpicalc/libpicalc_dll.h"
#endif // COMPONENT_BUILD

#ifndef COMPONENT_BUILD
void printStaticPiForTesting() {
  bool oss_success = false;
  bool woss_success = false;
  std::ostringstream osspi;
  std::wostringstream wosspi;
  if (oss_pi_chudnovsky(osspi)) {
    oss_success = true;
  }
  if (woss_pi_chudnovsky(wosspi)){
    woss_success = true;
  }
  std::cout << std::endl;
  std::cout << "oss_pi_chudnovsky = " << std::fixed << std::setprecision(1000) << osspi.str()
                                      << "\n" << std::endl;
  std::wcout << L"woss_pi_chudnovsky = " << wosspi.str() << L"\n" << std::endl;

  std::string* string_pi = string_pi_chudnovsky();
  std::cout << "string_pi_chudnovsky = " << string_pi->c_str() << "\n" << std::endl;
  string_pi = nullptr;

  std::wstring* wstring_pi = wstring_pi_chudnovsky();
  std::wcout << L"wstring_pi_chudnovsky = " << wstring_pi->c_str() << L"\n" << std::endl;
  wstring_pi = nullptr;

  char* char_pi = char_pi_chudnovsky();
  std::cout << "char_pi_chudnovsky = " << char_pi_chudnovsky() << "\n" << std::endl;
  delete[] char_pi;
  char_pi = nullptr;

  wchar_t* wchar_pi = wchar_pi_chudnovsky();
  std::wcout << L"wchar_pi_chudnovsky = " << wchar_pi_chudnovsky() << L"\n" << std::endl;
  delete[] wchar_pi;
  wchar_pi = nullptr;

  static const bool success =
      oss_success && woss_success;
  if (!success) {
    std::wcerr << __PRETTY_FUNCTION__ << L" Failed at one or more steps" << std::endl;
  }
}
#else
void printStaticPiForTesting() {
  return;
}
#endif // COMPONENT_BUILD

void printSharedPiForTesting(HMODULE hDll) {
  bool char_pi_succ = false;
  bool wchar_pi_succ = false;
  bool string_pi_succ = false;
  bool wstring_pi_succ = false;
  if (hDll != nullptr) {
    std::cout << std::endl;
  }

  CHAR_PI pCHAR_PI = reinterpret_cast<CHAR_PI>(
      GetProcAddress(hDll, "char_pi_chudnovsky")
  );
  if (!pCHAR_PI || pCHAR_PI == nullptr) {
    std::wcerr << L"Failed to get char_pi_chudnovsky function address!" << L"\n" << ENDL;
  } else {
    std::cout << "char_pi_chudnovsky = " << pCHAR_PI() << "\n" << ENDL;
    char_pi_succ = true;
  }

  WCHAR_PI pWCHAR_PI = reinterpret_cast<WCHAR_PI>(
      GetProcAddress(hDll, "wchar_pi_chudnovsky")
  );
  if (!pWCHAR_PI || pWCHAR_PI == nullptr) {
    std::wcerr << L"Failed to get wchar_pi_chudnovsky function address!" << L"\n" << ENDL;
  } else {
    std::wcout << L"wchar_pi_chudnovsky = " << pWCHAR_PI() << L"\n" << ENDL;
    wchar_pi_succ = true;
  }

  STRING_PI pSTRING_PI = reinterpret_cast<STRING_PI>(
      GetProcAddress(hDll, "string_pi_chudnovsky")
  );
  if (!pSTRING_PI || pSTRING_PI == nullptr) {
    std::wcerr << L"Failed to get string_pi_chudnovsky function address!" << L"\n" << ENDL;
  } else {
    std::cout << "string_pi_chudnovsky = " << pSTRING_PI()->c_str() << "\n" << ENDL;
    string_pi_succ = true;
  }

  WSTRING_PI pWSTRING_PI = reinterpret_cast<WSTRING_PI>(
      GetProcAddress(hDll, "wstring_pi_chudnovsky")
  );
  if (!pWSTRING_PI || pWSTRING_PI == nullptr) {
    std::wcerr << L"Failed to get wstring_pi_chudnovsky function address!" << L"\n" << ENDL;
  } else {
    std::wcout << L"wstring_pi_chudnovsky = " << pWSTRING_PI()->c_str() << L"\n" << ENDL;
    wstring_pi_succ = true;
  }

  static const bool success = char_pi_succ && wchar_pi_succ &&
                              string_pi_succ && wstring_pi_succ;
  if (!success) {
    std::wcerr << __PRETTY_FUNCTION__ << L" Failed at one or more steps" << std::endl;
  }
}
