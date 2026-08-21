#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "hp/math/add.hpp"
#include "hp/math/sub.hpp"
#include "hp/math/mult.hpp"
#include "hp/math/div.hpp"
#include "hp/math/calculator.hpp"
#include "hp/flows/overflow.hpp"

namespace hp {

    namespace make {

        template <typename T>
        constexpr T positive(T a) {
            static_assert(std::is_arithmetic_v<T>, "Argument must be arithmetic");
            if (a > 0) { return a; }
            if (a < 0) {
                if (a == std::numeric_limits<T>::min()) {
                    return std::numeric_limits<T>::max();
                    }
                return -a;
                }
            return 0;
            }

        // ----- Convert to negative
        template <typename T>
        constexpr T negative(T a) {
            static_assert(std::is_arithmetic_v<T>, "Argument must be arithmetic");
            if (a < 0) { return a; }
            if (a > 0) {
                if (a == std::numeric_limits<T>::max()) {
                    return std::numeric_limits<T>::min();
                    }
                return -a;
                }
            return 0;
            }

        template <typename T>
        constexpr auto toInt(T a) -> int {
            static_assert(!std::is_floating_point_v<T> || !std::integral<T>, "T should be Int or Float");
            if constexpr (std::is_floating_point_v<T>) {
                if (a > static_cast<T>(std::numeric_limits<int>::max()) ||
                    a < static_cast<T>(std::numeric_limits<int>::min())) {
                    return (a > 0) ? std::numeric_limits<int>::max()
                        : std::numeric_limits<int>::min();
                    }
                return static_cast<int>(a);
                }
            else {
                return a;
                }
            }
        template <typename T>
        constexpr auto toFloat(T a) -> float {
            static_assert(!std::is_floating_point_v<T> || !std::integral<T>, "T should be Int or Float");
            if constexpr (std::integral<T>) {
                if (a > static_cast<T>(std::numeric_limits<float>::max()) || 
                a < static_cast<T>(std::numeric_limits<float>::lowest())) {
                    return (a > 0) ? std::numeric_limits<float>::max()
                        : std::numeric_limits<float>::lowest();
                    }
                return static_cast<float>(a);
                }
            else {
                return a; 
                }
            }
        
        }
    // ----- Check if string is a number (including decimals)
    inline bool isNumber(const std::string& str) {
        if (str.empty()) return false;

        std::string trimmed = str;
        trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), ' '), trimmed.end());
        if (trimmed.empty()) return false;

        bool hasDecimal = false;
        size_t i = 0;

        // Handle optional sign
        if (trimmed[0] == '-' || trimmed[0] == '+') i++;
        if (i >= trimmed.length()) return false; 

        for (; i < trimmed.length(); i++) {
            if (trimmed[i] == '.') {
                if (hasDecimal) return false;
                hasDecimal = true;
                }
            else if (!std::isdigit(static_cast<unsigned char>(trimmed[i]))) {
                return false;
                }
            }
        return true;
        }

    // ----- Check if number is even
    inline bool isEven(int n) {
        return n % 2 == 0;
        }

    // ----- Check if number is odd
    inline bool isOdd(int n) {
        return n % 2 != 0;
        }

    // ----- Check if number is prime
    inline bool isPrime(int n) {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; i <= std::sqrt(n); i += 2) {
            if (n % i == 0) return false;
            }
        return true;
        }

    // ----- Calculate factorial (iterative)
    inline unsigned long long factorial(int n) {
        if (n < 0) return 0;
        unsigned long long result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i;
            }
        return result;
        }

    // ----- Calculate factorial (recursive)
    inline unsigned long long factorialRecursive(int n) {
        if (n < 0) return 0;
        if (n <= 1) return 1;
        return n * factorialRecursive(n - 1);
        }

    // ----- Greatest Common Divisor (Euclidean algorithm)
    inline int gcd(int a, int b) {
        a = std::abs(a);
        b = std::abs(b);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
            }
        return a;
        }

    // ----- Least Common Multiple
    inline int lcm(int a, int b) {
        if (a == 0 || b == 0) return 0;
        return std::abs(a) / gcd(a, b) * std::abs(b);
        }

    // ----- Generate nth Fibonacci number (iterative)
    inline unsigned long long fibonacci(int n) {
        if (n < 0) return 0;
        if (n <= 1) return n;
        unsigned long long a = 0, b = 1, c;
        for (int i = 2; i <= n; i++) {
            c = a + b;
            a = b;
            b = c;
            }
        return b;
        }

    // ----- Fibonacci Recursive
    inline unsigned long long fibonacciRecursive(int n) {
        if (n <= 1) return n;
        unsigned long long a = 0, b = 1;
        for (int i = 2; i <= n; i++) {
            unsigned long long temp = a + b;
            a = b;
            b = temp;
            }
        return b;
        }

    }



    