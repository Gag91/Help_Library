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

/*
----- Notes :
I suck at math..., so most of the formula have been founded whit google lol
*/
namespace hp {

    // ----- Check if string is a number (including decimals)
    bool isNumber(const std::string& str) {
        if (str.empty()) return false;

        std::string trimmed = str;
        trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), ' '), trimmed.end());
        if (trimmed.empty()) return false;

        bool hasDecimal = false;
        size_t i = 0;

        // Handle optional sign
        if (trimmed[0] == '-' || trimmed[0] == '+') i++;
        if (i >= trimmed.length()) return false;  // Just a sign?

        for (; i < trimmed.length(); i++) {
            if (trimmed[i] == '.') {
                if (hasDecimal) return false;  // Second decimal point
                hasDecimal = true;
                }
            else if (!std::isdigit(static_cast<unsigned char>(trimmed[i]))) {
                return false;
                }
            }
        return true;
        }

    // ----- Check if number is even
    bool isEven(int n) {
        return n % 2 == 0;
        }

    // ----- Check if number is odd
    bool isOdd(int n) {
        return n % 2 != 0;
        }

    // ----- Check if number is prime
    bool isPrime(int n) {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; i <= std::sqrt(n); i += 2) {
            if (n % i == 0) return false;
            }
        return true;
        }

    // ----- Calculate factorial (iterative)
    unsigned long long factorial(int n) {
        if (n < 0) return 0;
        unsigned long long result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i;
            }
        return result;
        }

    // ----- Calculate factorial (recursive)
    unsigned long long factorialRecursive(int n) {
        if (n < 0) return 0;
        if (n <= 1) return 1;
        return n * factorialRecursive(n - 1);
        }

    // ----- Greatest Common Divisor (Euclidean algorithm)
    int gcd(int a, int b) {
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
    int lcm(int a, int b) {
        if (a == 0 || b == 0) return 0;
        return std::abs(a) / gcd(a, b) * std::abs(b);
        }

    // ----- Generate nth Fibonacci number (iterative)
    unsigned long long fibonacci(int n) {
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
    unsigned long long fibonacciRecursive(int n) {
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



    