#ifndef LIBPICALC_LIBPICALC_DLL_H
#define LIBPICALC_LIBPICALC_DLL_H

#include "component_export.h"

// Exported functions for use outside the dll
COMPONENT_EXPORT bool oss_pi_chudnovsky(std::ostringstream &osspi);

COMPONENT_EXPORT bool woss_pi_chudnovsky(std::wostringstream &wosspi);

COMPONENT_EXPORT bool wpi_chudnovsky(wchar_t &wcpi);

COMPONENT_EXPORT BOOL WINAPI DllMain(HINSTANCE hDll,   // handle to DLL module
                                     DWORD reason,     // reason for calling function
                                     LPVOID reserved); // reserved

#endif // LIBPICALC_LIBPICALC_DLL_H
