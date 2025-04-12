#ifndef CHUDNOVSKY_H_
#define CHUDNOVSKY_H_

#include <cmath>
#include <cwchar>

constexpr int iterations = 28;

// Simple factorial function
long double factorial(long double num);

// Chudnovsky Algorithm for Pi calculation
long double chudnovsky(int iterations);

// Convert long double to wide character string
void convertDoubleToWString();

#endif // CHUDNOVSKY_H_
