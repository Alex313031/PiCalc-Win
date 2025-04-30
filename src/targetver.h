#pragma once

#ifndef TARGETVER_H_
#define TARGETVER_H_

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <WinSDKVer.h>

// Target Windows XP/Server 2003 as a minimum, Windows 2000 support in the future
//#define NTDDI_VERSION NTDDI_WINXP
#define WINVER _WIN32_WINNT_WINXP
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define _WIN64_WINNT _WIN32_WINNT_WS03s // Minimum version for 64 bit
#define _WIN32_IE 0x0500 // Minimum Internet Explorer version

// See https://tedwvc.wordpress.com/2014/01/01/how-to-target-xp-with-vc2012-or-vc2013-and-continue-to-use-the-windows-8-x-sdk/
//#define _USING_V110_SDK71_
#define _ATL_XP_TARGETING 

// Include this AFTER defining the minimum WINNT version you want
#include <SDKDDKVer.h>

#endif // TARGETVER_H_
