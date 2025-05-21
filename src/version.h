#ifndef VERSION_H_
#define VERSION_H_

// Macro to convert to string
#if !defined(_STRINGIZER) && !defined(STRINGIZE)
 #define _STRINGIZER(in) #in
 #define STRINGIZE(in) _STRINGIZER(in)
#endif

// Main version constant
#ifndef _VERSION
// Run stringizer above
#define _VERSION(major,minor,build) STRINGIZE(major) "." STRINGIZE(minor) "." STRINGIZE(build)
#endif

// These next few lines are where we control version number and copyright year
// Adhere to semver > semver.org
#define MAJOR_VERSION 0
#define MINOR_VERSION 4
#define BUILD_VERSION 1

#ifndef VERSION_STRING
#define VERSION_STRING _VERSION(MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION)
#define ABOUT_VERSION "PiCalc ver. 0.4.1"
#define ABOUT_COPYRIGHT "Copyright © 2025 Alex313031"
#define LEGAL_COPYRIGHT "© 2025 Alex313031 (BSD-3)"
#endif

#endif // VERSION_H_
