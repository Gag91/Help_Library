#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <cctype>
#include "hp/help.hpp"
#include <string_view>

class BigInt
{
private:
    std::vector<int> nums;
    bool negative = false;

    void trim()
    {
        while (nums.size() > 1 && nums.back() == 0)
        {
            nums.pop_back();
        }
        if (nums.size() == 1 && nums[0] == 0)
        {
            negative = false;
        }
    }

    int absCompare(const BigInt &other) const
    {
        if (nums.size() != other.nums.size())
        {
            return nums.size() < other.nums.size() ? -1 : 1;
        }
        for (int i = static_cast<int>(nums.size()) - 1; i >= 0; --i)
        {
            if (nums[i] != other.nums[i])
            {
                return nums[i] < other.nums[i] ? -1 : 1;
            }
        }
        return 0;
    }

    BigInt absAdd(const BigInt &other) const
    {
        BigInt result;
        result.nums.clear();

        int carry = 0;
        size_t max_size = std::max(nums.size(), other.nums.size());

        for (size_t i = 0; i < max_size || carry; ++i)
        {
            int sum = carry;
            if (i < nums.size())
                sum += nums[i];
            if (i < other.nums.size())
                sum += other.nums[i];

            result.nums.push_back(sum % 10);
            carry = sum / 10;
        }

        result.trim();
        return result;
    }

    BigInt absSub(const BigInt &other) const
    {
        BigInt result;
        result.nums.clear();

        int borrow = 0;
        for (size_t i = 0; i < nums.size(); ++i)
        {
            int diff = nums[i] - borrow;
            if (i < other.nums.size())
            {
                diff -= other.nums[i];
            }

            if (diff < 0)
            {
                diff += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }

            result.nums.push_back(diff);
        }

        result.trim();
        return result;
    }

    BigInt absMult(const BigInt &other) const
    {
        if (isZero() || other.isZero())
        {
            return BigInt(0);
        }

        BigInt result;
        result.nums.assign(nums.size() + other.nums.size(), 0);

        for (size_t i = 0; i < nums.size(); ++i)
        {
            int carry = 0;
            for (size_t j = 0; j < other.nums.size(); ++j)
            {
                int product = result.nums[i + j] + nums[i] * other.nums[j] + carry;
                result.nums[i + j] = product % 10;
                carry = product / 10;
            }
            if (carry > 0)
            {
                result.nums[i + other.nums.size()] = carry;
            }
        }

        result.trim();
        return result;
    }

    BigInt absDiv(const BigInt &other) const
    {
        if (other.isZero())
        {
            throw std::runtime_error("Cannot divide by 0");
        }

        if (other > *this)
        {
            return BigInt(0);
        }

        BigInt remainder;
        BigInt quotient;
        quotient.nums.assign(nums.size(), 0);

        for (int i = static_cast<int>(nums.size()) - 1; i >= 0; --i)
        {
            remainder.nums.insert(remainder.nums.begin(), nums[i]);
            remainder.trim();

            int digit = 0;
            while (true)
            {
                BigInt test = other * BigInt(digit + 1);
                if (test <= remainder)
                {
                    digit++;
                }
                else
                {
                    break;
                }
            }

            quotient.nums[i] = digit;
            if (digit > 0)
            {
                BigInt subtract = other * BigInt(digit);
                remainder = remainder - subtract;
            }
        }

        quotient.trim();
        return quotient;
    }

    BigInt absPow(const BigInt &other) const
    {
        if (other.isZero())
        {
            return BigInt(1);
        }
        if (isZero())
        {
            return BigInt(0);
        }

        BigInt result = 1;
        BigInt base = *this;
        BigInt exp = other;

        while (exp > 0)
        {
            if (exp % 2 == 1)
            {
                result *= base;
            }
            base = base * base;
            exp /= 2;
        }
        return result;
    }

public:
    BigInt() : nums{0}, negative(false) {}

    // ----- Copy Constructor
    BigInt(const BigInt &other)
    {
        nums = other.nums;
        negative = other.negative;
    }

    // ----- Move Constructor
    BigInt(BigInt &&other) noexcept
    {
        nums = std::move(other.nums);
        negative = other.negative;
    }

    // ----- Copy Assignment
    BigInt &operator=(const BigInt &other)
    {
        if (this != &other)
        {
            nums = other.nums;
            negative = other.negative;
        }
        return *this;
    }

    // ----- Move Assignment
    BigInt &operator=(BigInt &&other) noexcept
    {
        nums = std::move(other.nums);
        negative = other.negative;
        return *this;
    }

    BigInt(int value) : BigInt(static_cast<long long>(value)) {}
    BigInt(long value) : BigInt(static_cast<long long>(value)) {}

    BigInt(long long value)
    {
        if (value == 0)
        {
            nums.push_back(0);
            negative = false;
            return;
        }
        if (value < 0)
        {
            negative = true;
            value = -value;
        }
        while (value > 0)
        {
            nums.push_back(value % 10);
            value /= 10;
        }
        trim();
    }

    BigInt(const std::string_view &str)
    {
        if (str.empty())
        {
            throw std::invalid_argument("BigInt: string cannot be empty");
        }

        size_t start = 0;
        if (str[0] == '-')
        {
            negative = true;
            start = 1;
        }
        else if (str[0] == '+')
        {
            negative = false;
            start = 1;
        }

        if (start >= str.length())
        {
            throw std::invalid_argument("BigInt: missing digits after sign");
        }

        for (size_t i = start; i < str.length(); ++i)
        {
            unsigned char c = static_cast<unsigned char>(str[i]);
            if (!std::isdigit(c))
            {
                throw std::invalid_argument("BigInt: non-digit character");
            }
        }
        bool has_non_zero = false;
        for (size_t i = start; i < str.length(); ++i)
        {
            char c = str[i];
            if (!has_non_zero && c == '0')
            {
                continue;
            }
            has_non_zero = true;
            nums.push_back(c - '0');
        }

        if (!has_non_zero)
        {
            nums.push_back(0);
            negative = false;
        }
        std::reverse(nums.begin(), nums.end());

        trim();
    }

    BigInt(const std::string &str) : BigInt(std::string_view(str)) {}
    BigInt(const char *str) : BigInt(std::string_view(str)) {}

    bool isZero() const
    {
        return nums.size() == 1 && nums[0] == 0;
    }

    // ----- Conversion
    std::string toString() const
    {
        if (isZero())
            return "0";

        std::string result;
        for (int i = static_cast<int>(nums.size()) - 1; i >= 0; --i)
        {
            result += char('0' + nums[i]);
        }
        if (negative)
            result = "-" + result;
        return result;
    }

    int toInt() const
    {
        if (isZero())
            return 0;

        BigInt min_int = std::numeric_limits<int>::min();
        BigInt max_int = std::numeric_limits<int>::max();

        if (*this < min_int || *this > max_int)
        {
            throw std::overflow_error("BigInt value too large for int");
        }

        return std::stoi(toString());
    }

    // Convert to long (with overflow check)
    long toLong() const
    {
        if (isZero())
            return 0;

        BigInt min_long = std::numeric_limits<long>::min();
        BigInt max_long = std::numeric_limits<long>::max();

        if (*this < min_long || *this > max_long)
        {
            throw std::overflow_error("BigInt value too large for long");
        }

        return std::stol(toString());
    }

    // Convert to long long (with overflow check)
    long long toLLong() const
    {
        if (isZero())
            return 0;

        BigInt min_ll = std::numeric_limits<long long>::min();
        BigInt max_ll = std::numeric_limits<long long>::max();

        if (*this < min_ll || *this > max_ll)
        {
            throw std::overflow_error("BigInt value too large for long long");
        }

        return std::stoll(toString());
    }

    // ----- Unary Operators
    BigInt operator+() const
    {
        return *this;
    }

    BigInt operator-() const
    {
        BigInt result = *this;
        if (!result.isZero())
        {
            result.negative = !result.negative;
        }
        return result;
    }

    // ----- Assignment
    BigInt &operator=(const std::string &str)
    {
        *this = BigInt(str);
        return *this;
    }

    BigInt &operator=(const char *str)
    {
        *this = BigInt(str);
        return *this;
    }

    BigInt &operator=(long long value)
    {
        *this = BigInt(value);
        return *this;
    }

    BigInt &operator=(int value)
    {
        *this = BigInt(value);
        return *this;
    }

    // ----- Comparison Operators
    bool operator==(const BigInt &other) const
    {
        if (negative != other.negative)
            return false;
        if (nums.size() != other.nums.size())
            return false;
        return nums == other.nums;
    }

    bool operator!=(const BigInt &other) const
    {
        return !(*this == other);
    }

    bool operator<(const BigInt &other) const
    {
        if (negative != other.negative)
        {
            return negative;
        }

        int cmp = absCompare(other);
        if (cmp == 0)
            return false;
        return negative ? cmp > 0 : cmp < 0;
    }

    bool operator>(const BigInt &other) const
    {
        return other < *this;
    }

    bool operator<=(const BigInt &other) const
    {
        return !(*this > other);
    }

    bool operator>=(const BigInt &other) const
    {
        return !(*this < other);
    }

    // ----- Arithmetic Operators | BigInt +-*/ BigInt
    BigInt operator+(const BigInt &other) const
    {
        if (!negative && !other.negative)
        {
            return absAdd(other);
        }

        if (negative && other.negative)
        {
            BigInt result = absAdd(other);
            result.negative = true;
            return result;
        }

        if (!negative && other.negative)
        {
            BigInt abs_other = other;
            abs_other.negative = false;
            if (*this >= abs_other)
            {
                return absSub(abs_other);
            }
            else
            {
                BigInt result = abs_other.absSub(*this);
                result.negative = true;
                return result;
            }
        }

        BigInt abs_this = *this;
        abs_this.negative = false;
        if (abs_this >= other)
        {
            BigInt result = abs_this.absSub(other);
            result.negative = true;
            return result;
        }
        else
        {
            return other.absSub(abs_this);
        }
    }

    BigInt operator-(const BigInt &other) const
    {
        return *this + (-other);
    }

    BigInt operator*(const BigInt &other) const
    {
        if (isZero() || other.isZero())
        {
            return BigInt(0);
        }
        BigInt result = absMult(other);
        result.negative = negative != other.negative;
        result.trim();
        return result;
    }

    BigInt operator/(const BigInt &other) const
    {

        BigInt abs_this = *this;
        abs_this.negative = false;

        BigInt abs_other = other;
        abs_other.negative = false;

        BigInt result = abs_this.absDiv(abs_other);

        if (negative != other.negative)
        {
            result.negative = true;
        }
        return result;
    }

    BigInt operator%(const BigInt &other) const
    {
        return *this - (*this / other) * other;
    }

    BigInt operator^(const BigInt &other) const
    {

        BigInt abs_this = *this;
        abs_this.negative = false;
        BigInt result = abs_this.absPow(other);
        if (negative && (other % 2 == 1))
        {
            result.negative = true;
        }

        return result;
    }

    // ----- Arithmetic operators | BigInt +-*/ Intenger
    BigInt operator+(const long long &other)
    {
        return *this + BigInt(other);
    }

    BigInt operator-(const long long &other)
    {
        return *this - BigInt(other);
    }

    BigInt operator*(const long long &other)
    {
        return *this * BigInt(other);
    }

    BigInt operator/(const long long &other)
    {
        return *this / BigInt(other);
    }

    BigInt operator%(const long long &other)
    {
        return *this % BigInt(other);
    }

    BigInt operator^(const long long &other)
    {
        return *this ^ BigInt(other);
    }

    // ----- Compound Assignment
    BigInt &operator+=(const BigInt &other)
    {
        *this = *this + other;
        return *this;
    }

    BigInt &operator-=(const BigInt &other)
    {
        *this = *this - other;
        return *this;
    }

    BigInt &operator*=(const BigInt &other)
    {
        *this = *this * other;
        return *this;
    }

    BigInt &operator/=(const BigInt &other)
    {
        *this = *this / other;
        return *this;
    }

    BigInt &operator^=(const BigInt &other)
    {
        *this = *this ^ other;
        return *this;
    }

    BigInt &operator+=(const long long &other)
    {
        *this = *this + BigInt(other);
        return *this;
    }

    BigInt &operator-=(const long long &other)
    {
        *this = *this - BigInt(other);
        return *this;
    }

    BigInt &operator*=(const long long &other)
    {
        *this = *this * BigInt(other);
        return *this;
    }

    BigInt &operator/=(const long long &other)
    {
        *this = *this / BigInt(other);
        return *this;
    }

    BigInt &operator^=(const long long &other)
    {
        *this = *this ^ BigInt(other);
        return *this;
    }

    // ----- Increment/Decrement
    BigInt &operator++()
    {
        *this += 1;
        return *this;
    }

    BigInt operator++(int)
    {
        BigInt temp = *this;
        ++(*this);
        return temp;
    }

    BigInt &operator--()
    {
        *this -= 1;
        return *this;
    }

    BigInt operator--(int)
    {
        BigInt temp = *this;
        --(*this);
        return temp;
    }

    // ----- Stream Operators
    friend std::ostream &operator<<(std::ostream &out, const BigInt &other)
    {
        out << other.toString();
        return out;
    }

    friend std::istream &operator>>(std::istream &in, BigInt &other)
    {
        std::string input;
        in >> input;
        other = BigInt(input);
        return in;
    }

    // ----- Functions | BigInt & BigInt

    // ----- Absolute Value
    static BigInt abs(const BigInt &other)
    {
        return other < 0 ? -other : other;
    }

    // ----- ^10
    static BigInt pow_10(size_t exp)
    {
        BigInt result;
        result.nums.assign(exp, 0);
        result.nums.push_back(1);
        return result;
    }

    // ----- Square root
    static BigInt sqrt(const BigInt &other)
    {
        if (other < 0)
            throw std::invalid_argument("Cannot compute square root of a negative integer");

        if (other == 0)
            return 0;
        else if (other < 4)
            return 1;
        else if (other < 9)
            return 2;
        else if (other < 16)
            return 3;

        BigInt sqrt_prev = -1;
        BigInt sqrt_current = pow_10(other.toString().size() / 2 - 1);

        while (abs(sqrt_current - sqrt_prev) > 1)
        {
            sqrt_prev = sqrt_current;
            sqrt_current = (other / sqrt_prev + sqrt_prev) / 2;
        }
        return sqrt_current;
    }

    // ----- Greatest Common Divisor (Euclidean algorithm)
    static BigInt gcd(const BigInt &a, const BigInt &b)
    {
        BigInt aa = BigInt::abs(a);
        BigInt bb = BigInt::abs(b);
        while (!bb.isZero())
        {
            BigInt temp = bb;
            bb = aa % bb;
            aa = temp;
        }
        return aa;
    }

    static BigInt lcm(const BigInt &a, const BigInt &b)
    {
        if (a == 0 || b == 0)
            return 0;
        BigInt aa = BigInt::abs(a);
        BigInt bb = BigInt::abs(b);
        BigInt cc = gcd(aa, bb);
        return (aa / cc) * bb;
    }

    // ----- BigInt & Intenger
    static BigInt gcd(const BigInt &num1, const long long &num2)
    {
        return BigInt::gcd(BigInt(num1), BigInt(num2));
    }

    // ----- BigInt & String
    static BigInt gcd(const BigInt &num1, const std::string &num2)
    {
        return BigInt::gcd(num1, BigInt(num2));
    }

    // ----- BigInt & Intenger
    static BigInt gcd(const long long &num1, const BigInt &num2)
    {
        return BigInt::gcd(BigInt(num1), num2);
    }

    // ----- String & BigInt
    static BigInt gcd(const std::string &num1, const BigInt &num2)
    {
        return BigInt::gcd(BigInt(num1), num2);
    }

    // ----- Least Common Multiple
    static BigInt lcm(const long long &num1, const BigInt &num2)
    {
        return BigInt::lcm(BigInt(num1), num2);
    }

    static BigInt lcm(const std::string &num1, const BigInt &num2)
    {
        return BigInt::lcm(BigInt(num1), num2);
    }

    size_t size() const
    {
        return nums.size();
    }
};

BigInt operator""_big(const char *str, size_t len)
{
    return BigInt(std::string(str, len));
}

// ----- Arithmetic operators | Intenger +-*/ BigInt
BigInt operator+(const long long &num, const BigInt &other)
{
    return BigInt(num) + other;
}

BigInt operator-(const long long &num, const BigInt &other)
{
    return BigInt(num) - other;
}

BigInt operator*(const long long &num, const BigInt &other)
{
    return BigInt(num) * other;
}

BigInt operator/(const long long &num, const BigInt &other)
{
    return BigInt(num) / other;
}
BigInt operator^(const long long &num, const BigInt &other)
{
    return BigInt(num) ^ other;
}
