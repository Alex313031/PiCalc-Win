#include "common.h"
#include "resource.h"

std::wstring getVersionString() {
  std::wstringstream VerStream;
  VerStream << MAJOR_VERSION
            << "."
            << MINOR_VERSION
            << "."
            << BUILD_VERSION;
  return VerStream.str();
}
