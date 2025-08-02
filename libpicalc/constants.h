// Define float128 differently depending on compiler
#ifndef __float128
 typedef long double float128;
#else
 typedef __float128 float128;
#endif // __float128

// Exit codes
#ifndef SUCCESS_CODE
 #define SUCCESS_CODE 0
#endif
#ifndef ERROR_CODE
 #define ERROR_CODE 1
#endif

// Easier line endings
#ifndef ENDL
 #define ENDL std::endl
#endif // ENDL
#ifndef NL
 // Newline
 #define NL "\n"
 // "wide" newline
 #define WNL L"\n"
#endif // ENDL

// Maximum .res loadstring length
#ifndef MAX_LOADSTRING
 #define MAX_LOADSTRING MAX_PATH // Traditionally 255
#endif // MAX_LOADSTRING

constexpr int SUCCESS = SUCCESS_CODE;
constexpr int FAILED = ERROR_CODE;

constexpr float128 fzero = 0.0;

// Bool to be used externally to test if DCHECK is on
constexpr bool is_dcheck = DCHECK && DCHECK == 1;

// For testing the different trap functions
const bool test_trap = false;

// Same as above but for DEBUG/NDEBUG
#if defined DEBUG || defined _DEBUG
 constexpr bool is_debug = true;
#else
 constexpr bool is_debug = false;
#endif // defined DEBUG || defined DEBUG

constexpr long double c = 299792458.0; // Speed of light in m/s
