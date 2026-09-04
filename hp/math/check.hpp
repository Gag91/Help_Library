#pragma once

#include <limits>
#include <type_traits>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <string>

namespace hp
{
    namespace math
    {
        namespace check
        {
            // ----- Check if string is a number (including decimals)
            inline bool isNumber(const std::string &str)
            {
                if (str.empty())
                    return false;

                std::string trimmed = str;
                trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), ' '), trimmed.end());
                if (trimmed.empty())
                    return false;

                bool hasDecimal = false;
                std::size_t i = 0;

                if (trimmed[0] == '-' || trimmed[0] == '+')
                    i++;
                if (i >= trimmed.length())
                    return false;

                bool hasDigit = false;
                for (; i < trimmed.length(); i++)
                {
                    if (trimmed[i] == '.')
                    {
                        if (hasDecimal)
                            return false;
                        hasDecimal = true;
                    }
                    else if (std::isdigit(static_cast<unsigned char>(trimmed[i])))
                    {
                        hasDigit = true;
                    }
                    else
                    {
                        return false;
                    }
                }
                return hasDigit;
            }

            // ----- Check if number is even
            inline bool isEven(int n)
            {
                return n % 2 == 0;
            }

            // ----- Check if number is odd
            inline bool isOdd(int n)
            {
                return n % 2 != 0;
            }

            // ----- Check if number is prime
            inline bool isPrime(int n)
            {
                if (n < 2)
                    return false;
                if (n == 2)
                    return true;
                if (n % 2 == 0)
                    return false;
                for (int i = 3; i <= std::sqrt(n); i += 2)
                {
                    if (n % i == 0)
                        return false;
                }
                return true;
            }
        }
    }
}