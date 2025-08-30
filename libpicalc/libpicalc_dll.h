#ifndef LIBPICALC_LIBPICALC_DLL_H
#define LIBPICALC_LIBPICALC_DLL_H

#include "component_export.h"

// Exported functions for use outside the dll
COMPONENT_EXPORT bool oss_pi_chudnovsky(std::ostringstream &osspi);

COMPONENT_EXPORT bool woss_pi_chudnovsky(std::wostringstream &wosspi);

COMPONENT_EXPORT char* char_pi_chudnovsky();

COMPONENT_EXPORT wchar_t* wchar_pi_chudnovsky();

COMPONENT_EXPORT std::string* string_pi_chudnovsky();

COMPONENT_EXPORT std::wstring* wstring_pi_chudnovsky();

COMPONENT_EXPORT BOOL WINAPI DllMain(HINSTANCE hDll,   // handle to DLL module
                                     DWORD reason,     // reason for calling function
                                     LPVOID reserved); // reserved

#endif // LIBPICALC_LIBPICALC_DLL_H
