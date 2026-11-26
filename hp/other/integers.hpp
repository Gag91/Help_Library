#pragma once

#include <array>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace hp {
    // ----- Is digit
    [[nodiscard]] constexpr bool is_digit(char c) noexcept {
        return c >= '0' && c <= '9';
    }

    // ----- Floating point to string
    template <std::floating_point T>
    [[nodiscard]] constexpr std::string float_to_string(T val) {
        if consteval {
            if (val == 0) {
                return "0";
            }

            std::string res;
            if (val < 0) {
                res += '-';
                val = -val;
            }
            auto int_part = static_cast<unsigned long long>(val);
            T frac = val - static_cast<T>(int_part);

            if (int_part == 0) {
                res += '0';
            } else {
                std::string digits;
                while (int_part > 0) {
                    digits += static_cast<char>('0' + (int_part % 10));
                    int_part /= 10;
                }
                for (std::size_t i = digits.size(); i > 0; --i) {
                    res += digits[i - 1];
                }
            }

            if (frac > 0) {
                res += '.';
                for (int i = 0; i < 6 && frac > static_cast<T>(1e-7); ++i) {
                    frac *= 10;
                    int digit = static_cast<int>(frac);
                    res += static_cast<char>('0' + digit);
                    frac -= digit;
                }
            }
            return res;
        } else {
            std::array<char, 64> buf{};
            const auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), val);
            return std::string(buf.data(), ptr);
        }
    }

    // ----- String to Number
    template <typename T>
        requires std::is_arithmetic_v<T>
    [[nodiscard]] constexpr T to_number(std::string_view sv) {
        if (sv.empty()) {
            throw std::invalid_argument("hp::to_number: empty string");
        }

        if constexpr (std::integral<T>) {
            T val{};
            auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);
            if (ec == std::errc::result_out_of_range) {
                throw std::out_of_range("hp::to_number: value out of range");
            }
            if (ec != std::errc{} || ptr != sv.data() + sv.size()) {
                throw std::invalid_argument("hp::to_number: invalid integer");
            }
            return val;
        } else {
            if consteval {
                std::size_t i = 0;
                bool neg = false;
                if (sv[i] == '-') {
                    neg = true;
                    ++i;
                } else if (sv[i] == '+') {
                    ++i;
                }

                T res = 0;
                bool has_digits = false;
                while (i < sv.size() && is_digit(sv[i])) {
                    has_digits = true;
                    res = res * 10 + static_cast<T>(sv[i] - '0');
                    ++i;
                }

                if (i < sv.size() && sv[i] == '.') {
                    ++i;
                    T place = static_cast<T>(0.1);
                    while (i < sv.size() && is_digit(sv[i])) {
                        has_digits = true;
                        res += static_cast<T>(sv[i] - '0') * place;
                        place *= static_cast<T>(0.1);
                        ++i;
                    }
                }

                if (!has_digits || i != sv.size()) {
                    throw std::invalid_argument("hp::to_number: invalid floating-point number");
                }
                return neg ? -res : res;
            } else {
                T val{};
                auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);
                if (ec == std::errc::result_out_of_range) {
                    throw std::out_of_range("hp::to_number: value out of range");
                }
                if (ec != std::errc{} || ptr != sv.data() + sv.size()) {
                    throw std::invalid_argument("hp::to_number: invalid floating-point number");
                }
                return val;
            }
        }
    }

} // namespace hp