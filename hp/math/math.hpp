#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <limits>
#include "hp/math/add.hpp"
#include "hp/math/sub.hpp"
#include "hp/math/mult.hpp"
#include "hp/math/div.hpp"
#include "hp/math/calculator.hpp"
#include "hp/flows/overflow.hpp"
#include "hp/math/make.hpp"
#include "hp/math/check.hpp"

namespace hp
{

    // ----- Calculate factorial (iterative)
    inline unsigned long long factorial(int n)
    {
        if (n < 0)
            return 0;
        unsigned long long result = 1;
        for (int i = 2; i <= n; i++)
        {
            result *= i;
        }
        return result;
    }

    // ----- Calculate factorial (recursive)
    inline unsigned long long factorialRecursive(int n)
    {
        if (n < 0)
            return 0;
        if (n <= 1)
            return 1;
        return n * factorialRecursive(n - 1);
    }

    // ----- Greatest Common Divisor (Euclidean algorithm)
    inline int gcd(int a, int b)
    {
        a = std::abs(a);
        b = std::abs(b);
        while (b != 0)
        {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // ----- Least Common Multiple
    inline int lcm(int a, int b)
    {
        if (a == 0 || b == 0)
            return 0;
        return std::abs(a) / gcd(a, b) * std::abs(b);
    }

    // ----- Generate nth Fibonacci number (iterative)
    inline unsigned long long fibonacci(int n)
    {
        if (n < 0)
            return 0;
        if (n <= 1)
            return n;
        unsigned long long a = 0, b = 1, c;
        for (int i = 2; i <= n; i++)
        {
            c = a + b;
            a = b;
            b = c;
        }
        return b;
    }

    // ----- Fibonacci Recursive
    inline unsigned long long fibonacciRecursive(int n)
    {
        if (n <= 1)
            return n;
        unsigned long long a = 0, b = 1;
        for (int i = 2; i <= n; i++)
        {
            unsigned long long temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    }

}
