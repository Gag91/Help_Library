#pragma once

#include <limits>
#include <type_traits>
#include <ranges>
#include <concepts>
#include <optional>
#include <vector>

namespace hp {

    namespace overflow {
        namespace check {

            // ----- Check Addition Overflow
            template <typename T>
            constexpr bool Add(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::overflow::check::Add: T must be arithmetic!");

                if (a > 0 && b > 0) {
                    return a > std::numeric_limits<T>::max() - b;
                    }
                return false;
                }

            // ----- Check Subtraction Overflow
            template <typename T>
            constexpr bool Sub(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::overflow::check::Sub: T must be arithmetic!");

                if (a > 0 && b < 0) {
                    // Protect against INT_MIN
                    if (b == std::numeric_limits<T>::min()) {
                        return true;
                        }
                    T posB = -b;
                    return a > std::numeric_limits<T>::max() - posB;
                    }
                return false;
                }

            // ----- Check Multiplication Overflow
            template <typename T>
            constexpr bool Mult(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::overflow::check::Mult: T must be arithmetic!");

                if (a == 0 || b == 0) return false;

                if constexpr (std::is_integral_v<T>) {
                    if (a > 0 && b > 0) {
                        return a > std::numeric_limits<T>::max() / b;
                        }
                    if (a < 0 && b < 0) {
                        if (a == std::numeric_limits<T>::min() ||
                            b == std::numeric_limits<T>::min()) {
                            return true;
                            }
                        T posA = -a;
                        T posB = -b;
                        return posA > std::numeric_limits<T>::max() / posB;
                        }
                    return false;
                    }
                else if constexpr (std::is_floating_point_v<T>) {
                    if (a != 0 && b != 0) {
                        return std::abs(a) > std::numeric_limits<T>::max() / std::abs(b);
                        }
                    }
                return false;
                }

            // ----- Check Division (division by zero)
            template <typename T>
            constexpr bool Division_Zero(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::overflow::check::Div: T must be arithmetic!");

                return b == 0;
                }

            // ----- Check Division Overflow
            template <typename T>
            constexpr bool Div(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::overflow::check::Div: T must be arithmetic!");
                if (hp::overflow::check::Division_Zero(a, b)) {
                    return true;
                    }
                if constexpr (std::is_integral_v<T>) {
                    if (a == std::numeric_limits<T>::min() && b == -1) {
                        return true;
                        }
                    }
                return false;
                }

            } // namespace check

            // ----- Check if ANY operation would overflow
        template <typename T>
        constexpr bool any(T a, T b) {
            static_assert(std::is_arithmetic_v<T>,
                "hp::overflow::any: T must be arithmetic!");

            return check::Add(a, b) ||
                check::Sub(a, b) ||
                check::Mult(a, b);
            }

        // ----- Check if ALL operations are safe
        template <typename T>
        constexpr bool none(T a, T b) {
            static_assert(std::is_arithmetic_v<T>,
                "hp::overflow::none: T must be arithmetic!");

            return !any(a, b);
            }

        namespace make {

            // ----- Check if adding multiple numbers would overflow
            template <typename T, typename... Args>
            constexpr bool Add(T current, Args... rest) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::overflow::make::Add: First argument must be arithmetic!");
                static_assert((std::is_arithmetic_v<Args> && ...),
                    "hp::overflow::make::Add: All arguments must be arithmetic!");

                T sum = current;

                auto addOne = [&](auto value) -> bool {
                    if (check::Add(sum, value)) {
                        return true;
                        }
                    sum += value;
                    return false;
                    };

                return (addOne(rest) || ...);
                }

            // ----- Check if subtracting multiple numbers would overflow
            template <typename T, typename... Args>
            constexpr bool Sub(T current, Args... rest) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::overflow::make::Sub: First argument must be arithmetic!");
                static_assert((std::is_arithmetic_v<Args> && ...),
                    "hp::overflow::make::Sub: All arguments must be arithmetic!");

                T result = current;

                auto subOne = [&](auto value) -> bool {
                    if (check::Sub(result, value)) {
                        return true;
                        }
                    result -= value;
                    return false;
                    };

                return (subOne(rest) || ...);
                }

            // ----- Check if multiplying multiple numbers would overflow
            template <typename T, typename... Args>
            constexpr bool Mult(T current, Args... rest) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::overflow::make::Mult: First argument must be arithmetic!");
                static_assert((std::is_arithmetic_v<Args> && ...),
                    "hp::overflow::make::Mult: All arguments must be arithmetic!");

                T product = current;

                auto multOne = [&](auto value) -> bool {
                    if (check::Mult(product, value)) {
                        return true;
                        }
                    product *= value;
                    return false;
                    };

                return (multOne(rest) || ...);
                }

            } // namespace make

        } // namespace overflow

    } // namespace hp