/*
 * This code is distributed under the terms of the GNU General Public License.
 * For more information, please refer to the LICENSE file in the root directory.
 * -------------------------------------------------
 * Copyright (C) 2025 Rodrigo R.
 * This program comes with ABSOLUTELY NO WARRANTY; for details type show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type show c' for details.
*/
#ifndef FLUENT_LIBC_STD_MATH_LIBRARY_H
#define FLUENT_LIBC_STD_MATH_LIBRARY_H

// ============= FLUENT LIB C =============
/*
 * This code is distributed under the terms of the GNU General Public License.
 * For more information, please refer to the LICENSE file in the root directory.
 * -------------------------------------------------
 * Copyright (C) 2025 Rodrigo R.
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w`.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c` for details.
*/

// ============= FLUENT LIB C =============
// Custom Math Utilities
// ----------------------------------------
// A small utility collection of math-related functions that cover:
// - Integer comparisons
// - Custom `pow`, `floor`, and `fmod` implementations
// - Factorials
// - Taylor/Maclaurin series for sin, cos, and exp
//
// NOTE:
// Most of these functions are intended for **educational** or **demonstrative** purposes only.
// Especially the Taylor/Maclaurin series approximations (`taylor_sine`, `taylor_cosine`, `e_to_the_x`)
// should NOT be used in production due to performance and precision limitations.
//
// Features:
// Pure C99-compatible math utilities
// No reliance on `math.h` standard lib (can be used in freestanding environments)
// Includes manual `pow`, `floor`, `fmod`
// Includes angle normalization and factorial handling
//
// Use Cases:
// - Teaching basic numerical methods
// - Building minimal C runtime environments
// - Learning how math functions work under the hood
//
// Example:
// ----------------------------------------
//   double angle = 90.0;
//   double sin_val = taylor_sine(angle, 10); // ≈ 1.0
//   double e_power = e_to_the_x(1.0, 12);    // ≈ 2.718281828...

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif

// ============= INCLUDES =============
#ifndef FLUENT_LIBC_RELEASE
#   include <types.h> // fluent_libc
#else
#   include <fluent/types/types.h> // fluent_libc
#endif

#ifndef NAN
#   define NAN __builtin_nanf("")
#endif

// Define Euler's number
#ifndef EULER_NUMBER
#   define EULER_NUMBER 2.718282
#endif

// Handle definitions of PI and other numbers for 64-bit architectures
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(_M_ARM64)
    // 64-bit system
#   define M_PI 3.14159265358979323846  // Double precision
#elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) || defined(_M_ARM)
    // 32-bit system
#   define M_PI 3.1415927f  // Single precision (float)
#else
    // Unknown architecture
#   define M_PI 3.14159265358979323846  // Default (double precision)
#endif

#ifndef T_M_PI
#define T_M_PI M_PI * 2
#endif

/**
 * Compares two size_t values and returns the larger of the two.
 *
 * @param pivot The first value to compare.
 * @param target The second value to compare.
 * @return The larger of the two input values.
 */
static inline size_t num_max(const size_t pivot, const size_t target)
{
    if (pivot > target)
    {
        return pivot;
    }

    return target;
}

/**
 * Compares two size_t values and returns the smaller of the two.
 *
 * @param pivot The first value to compare.
 * @param target The second value to compare.
 * @return The smaller of the two input values.
 */
static inline size_t num_min(const size_t pivot, const size_t target)
{
    if (pivot > target)
    {
        return target;
    }

    return pivot;
}

/**
 * Calculates the power of a number using exponentiation by squaring.
 *
 * @param x The base value (double).
 * @param y The exponent value (sssize_t).
 * @return The result of raising `x` to the power of `y`.
 *
 * This function handles the following cases:
 * - If the exponent is 0, the result is 1.
 * - If the exponent is negative, the reciprocal of the base is used, and the exponent is made positive.
 * - The calculation is performed using an efficient iterative method.
 */
static inline double num_pow(double x, ssize_t y) {
    // Handle the case where exponent is 0
    if (y == 0) {
        return 1.0;
    }

    // If the exponent is negative, calculate the positive exponent and return the reciprocal
    if (y < 0) {
        x = 1 / x;  // Take reciprocal of x for negative exponents
        y = -y;     // Make the exponent positive
    }

    double result = 1.0;

    // Perform exponentiation by squaring iteratively
    while (y > 0) {
        if (y % 2 == 1) {  // If y is odd
            result *= x;
        }
        x *= x;  // Square the base
        y /= 2;  // Divide the exponent by 2
    }

    return result;
}

/**
 * Rounds a floating-point number down to the nearest integer.
 *
 * @param x The input floating-point number.
 * @return The largest integer less than or equal to the input value.
 *         If the input is already an integer, it is returned as is.
 *         For positive numbers, the integer part is returned.
 *         For negative numbers, the integer part minus one is returned.
 */
static inline double num_floor(const double x) {
    // If x is already an integer, return x
    if (x == (int)x) {
        return x;
    }

    // If x is positive, return the integer part by casting to int
    if (x > 0) {
        return (int)x;
    }

    // If x is negative, return the integer part minus 1
    return (int)x - 1;
}

/**
 * Computes the floating-point remainder of the division of `x` by `y`.
 *
 * This function calculates the remainder of `x / y` using the formula:
 * remainder = x - y * floor(x / y)
 *
 * @param x The dividend (double).
 * @param y The divisor (double). Must not be zero.
 * @return The remainder of the division as a double.
 */
static inline double num_fmod(const double x, const double y) {
    return x - y * num_floor(x / y);
}

/**
 * Calculates the factorial of a given non-negative integer.
 *
 * @param value The non-negative integer for which the factorial is to be calculated.
 *              If the value is 0 or 1, the factorial is defined as 1.
 * @return The factorial of the input value as a `size_t`.
 */
static inline size_t factorial(const size_t value)
{
    // Edge case for factorial of 0
    if (value == 0 || value == 1)
        return 1;

    // Define a result
    size_t result = 1;

    // Multiply linearly
    for (size_t i = 2; i <= value; i++)
    {
        result = result * i;
    }

    return result;
}

/**
 * Approximates the sine of a given angle using the Taylor series expansion.
 *
 * The Taylor series for sine is given by:
 * sin(x) ≈ Σ [(-1)^n * x^(2n+1) / (2n+1)!] for n = 0 to expansion_size
 *
 * @param value The angle in degrees for which the sine is to be approximated.
 * @param expansion_size The number of terms in the Taylor series expansion.
 *                        A higher value results in greater accuracy.
 * @return The approximated sine value of the input angle.
 */
static inline double taylor_sine(const double value, const size_t expansion_size)
{
    // Convert input value to radians if it's in degrees
    double rad_value = value * (M_PI / 180.0);

    // Normalize the value between -pi and pi
    rad_value = num_fmod(rad_value + M_PI, T_M_PI) - M_PI;

    // Define a result
    double result = 0;

    // Use the expansion to get a result
    for (ssize_t n = 0; n <= expansion_size; n++)
    {
        // Use the Taylor series: (-1)^n * x^(2n+1) / (2n+1)!
        const ssize_t exponent = 2 * n + 1;
        const double denominator = (double)factorial(exponent);  // factorial expects integers
        const double numerator = num_pow(rad_value, exponent);  // value raised to an odd exponent

        // Determine the sign (should alternate correctly)
        if (n % 2 == 0)
        {
            result += numerator / denominator;
        }
        else
        {
            result -= numerator / denominator;
        }
    }

    return result;
}

/**
 * Approximates the cosine of a given angle using the Taylor series expansion.
 *
 * The Taylor series for cosine is given by:
 * cos(x) ≈ Σ [(-1)^n * x^(2n) / (2n)!] for n = 0 to expansion_size
 *
 * @param value The angle in degrees for which the cosine is to be approximated.
 * @param expansion_size The number of terms in the Taylor series expansion.
 *                        A higher value results in greater accuracy.
 * @return The approximated cosine value of the input angle.
 */
static inline double taylor_cosine(const double value, const size_t expansion_size)
{
    // Convert input value to radians if it's in degrees
    double rad_value = value * (M_PI / 180.0);
    // Normalize the value between -pi and pi
    rad_value = num_fmod(rad_value + M_PI, T_M_PI) - M_PI;

    // Define a result
    double result = 0;

    // Use the expansion to get a result
    for (ssize_t n = 0; n <= expansion_size; n++)
    {
        // Use the Taylor series: (-1)^n * x^(2n) / (2n)!
        const ssize_t exponent = 2 * n;
        const double denominator = (double)factorial(exponent);  // factorial expects integers
        const double numerator = num_pow(rad_value, exponent);  // value raised to an even exponent

        // Determine the sign (should alternate correctly)
        if (n % 2 == 0)
        {
            result += numerator / denominator;
        }
        else
        {
            result -= numerator / denominator;
        }
    }

    return result;
}

/**
 * Approximates the value of e (Euler's number) raised to the power of `x`
 * using the Maclaurin series expansion.
 *
 * The Maclaurin series for e^x is given by:
 * e^x ≈ Σ [x^n / n!] for n = 0 to series_size
 *
 * @param x The exponent to which e is raised.
 * @param series_size The number of terms in the Maclaurin series expansion.
 *                    A higher value results in greater accuracy.
 * @return The approximated value of e^x as a double.
 */
static inline double e_to_the_x(const double x, const size_t series_size)
{
    // Edge case: 0
    if (x == 0)
    {
        return 1;
    }

    // Use the Maclaurin series to approximate the values
    // x^n/n!
    double result = 0;

    for (ssize_t n = 0; n <= series_size; n++)
    {
        // Calculate the exponent
        const double numerator = num_pow(x, n);
        const double denominator = (float) factorial(n);

        result += numerator / denominator;
    }

    return result;
}

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_STD_MATH_LIBRARY_H