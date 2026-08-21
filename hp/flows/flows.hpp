#include <optional>
#include "hp/flows/overflow.hpp"
#include "hp/flows/underflow.hpp"
namespace hp {
    namespace safe {
        namespace check {
            // ----- Addition (checks overflow + underflow)
            template <typename T>
            [[nodiscard]] constexpr bool Add(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::Add: Arguments must be arithmetic");

                return hp::overflow::check::Add(a, b) ||
                    hp::underflow::check::Add(a, b);
                }

            // ----- Subtraction (checks overflow + underflow)
            template <typename T>
            [[nodiscard]] constexpr bool Sub(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::Sub: Arguments must be arithmetic");

                return hp::overflow::check::Sub(a, b) ||
                    hp::underflow::check::Sub(a, b);
                }

            // ----- Multiplication (checks overflow + underflow)
            template <typename T>
            [[nodiscard]] constexpr bool Mult(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::Mult: Arguments must be arithmetic");

                return hp::overflow::check::Mult(a, b) ||
                    hp::underflow::check::Mult(a, b);
                }

            // ----- Division (checks division by zero + overflow)
            template <typename T>
            [[nodiscard]] constexpr bool Div(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::Div: Arguments must be arithmetic");

                return hp::overflow::check::Div(a, b) ||
                    hp::underflow::check::Div(a, b);
                }

            // ----- Check if ANY operation is safe
            template <typename T>
            [[nodiscard]] constexpr bool any(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::any: Arguments must be arithmetic");

                return Add(a, b) || Sub(a, b) || Mult(a, b) || Div(a, b);
                }

            // ----- Check if ALL operations are safe
            template <typename T>
            [[nodiscard]] constexpr bool none(T a, T b) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::none: Arguments must be arithmetic");

                return !any(a, b);
                }
            } // namespace check

        namespace make {
            // ----- Addition (checks overflow + underflow) | Multiple arguments
            template <typename T, typename... Args>
            [[nodiscard]] constexpr bool Add(T first, Args... rest) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::Add: First argument must be arithmetic");
                static_assert((std::is_arithmetic_v<Args> && ...),
                    "hp::safe::Add: All arguments must be arithmetic");

                return hp::overflow::make::Add(first, rest...) ||
                    hp::underflow::make::Add(first, rest...);
                }

            // ----- Subtraction (checks overflow + underflow) | Multiple arguments
            template <typename T, typename... Args>
            [[nodiscard]] constexpr bool Sub(T first, Args... rest) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::Sub: First argument must be arithmetic");
                static_assert((std::is_arithmetic_v<Args> && ...),
                    "hp::safe::Sub: All arguments must be arithmetic");

                return hp::overflow::make::Sub(first, rest...) ||
                    hp::underflow::make::Sub(first, rest...);
                }

            // ----- Multiplication (checks overflow + underflow) | Multiple arguments
            template <typename T, typename... Args>
            [[nodiscard]] constexpr bool Mult(T first, Args... rest) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::Mult: First argument must be arithmetic");
                static_assert((std::is_arithmetic_v<Args> && ...),
                    "hp::safe::Mult: All arguments must be arithmetic");

                return hp::overflow::make::Mult(first, rest...) ||
                    hp::underflow::make::Mult(first, rest...);
                }

            // ----- Division (checks division by zero + overflow) | Multiple arguments
            template <typename T, typename... Args>
            [[nodiscard]] constexpr bool Div(T first, Args... rest) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::Div: First argument must be arithmetic");
                static_assert((std::is_arithmetic_v<Args> && ...),
                    "hp::safe::Div: All arguments must be arithmetic");

                T result = first;

                auto divOne = [&](auto value) -> bool {
                    if (hp::safe::check::Div(result, value)) {
                        return true;
                        }
                    result /= value;
                    return false;
                    }

                return (divOne(rest) || ...);
                }

            // ----- Check if ANY operation would overflow/underflow (multiple args)
            template <typename T, typename... Args>
            [[nodiscard]] constexpr bool any(T first, Args... rest) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::any: First argument must be arithmetic");
                static_assert((std::is_arithmetic_v<Args> && ...),
                    "hp::safe::any: All arguments must be arithmetic");

                return Add(first, rest...) || Sub(first, rest...) ||
                    Mult(first, rest...) || Div(first, rest...);
                }


            // ----- Check if ALL operations are safe (multiple args)
            template <typename T, typename... Args>
            [[nodiscard]] constexpr bool none(T first, Args... rest) {
                static_assert(std::is_arithmetic_v<T>,
                    "hp::safe::none: First argument must be arithmetic");
                static_assert((std::is_arithmetic_v<Args> && ...),
                    "hp::safe::none: All arguments must be arithmetic");

                return !any(first, rest...);
                }
            } // namespace make

        namespace Try {
            namespace check {
                // ----- Try Addition (checks overflow + underflow)
                template <typename T>
                [[nodiscard]] constexpr std::optional<T> Add(T a, T b) {
                    if (hp::overflow::check::Add(a, b) || hp::underflow::check::Add(a, b)) {
                        return std::nullopt;
                        }
                    return a + b;
                    }

                // ----- Try Substraction (checks overflow + underflow)
                template <typename T>
                [[nodiscard]] constexpr std::optional<T> Sub(T a, T b) {
                    if (hp::overflow::check::Sub(a, b) || hp::underflow::check::Sub(a, b)) {
                        return std::nullopt;
                        }
                    return a - b;
                    }

                // ----- Try Multiplication (checks overflow + underflow)
                template <typename T>
                [[nodiscard]] constexpr std::optional<T> Mult(T a, T b) {
                    if (hp::overflow::check::Mult(a, b) || hp::underflow::check::Mult(a, b)) {
                        return std::nullopt;
                        }
                    return a * b;
                    }

                // ----- Try Division (checks overflow + underflow)
                template <typename T>
                [[nodiscard]] constexpr std::optional<T> Div(T a, T b) {
                    if (hp::overflow::check::Div(a, b) || hp::underflow::check::Div(a, b)) {
                        return std::nullopt;
                        }
                    return a / b;
                    }
                } // namespace check

            namespace make {

                // ----- Try Addition (checks overflow + underflow) | Multiple arguments
                template <typename T, typename... Args>
                [[nodiscard]] constexpr std::optional<T> Add(T a, Args...rest) {
                    if (hp::overflow::make::Add(a, rest...) || hp::underflow::make::Add(a, rest...)) {
                        return std::nullopt;
                        }
                    return (a + ... + rest);
                    }

                // ----- Try Substraction (checks overflow + underflow) | Multiple arguments
                template <typename T, typename... Args>
                [[nodiscard]] constexpr std::optional<T> Sub(T a, Args...rest) {
                    if (hp::overflow::make::Sub(a, rest...) || hp::underflow::make::Sub(a, rest...)) {
                        return std::nullopt;
                        }
                    return (a - ... - rest);
                    }
                // ----- Try Multiplication (checks overflow + underflow) | Multiple arguments
                template <typename T, typename... Args>
                [[nodiscard]] constexpr std::optional<T> Mult(T a, Args...rest) {
                    if (hp::overflow::make::Mult(a, rest...) || hp::underflow::make::Mult(a, rest...)) {
                        return std::nullopt;
                        }
                    return (a * ... * rest);
                    }

                // ----- Try Division (checks overflow + underflow) | Multiple arguments
                template <typename T, typename... Args>
                [[nodiscard]] constexpr std::optional<T> Div(T a, Args...rest) {
                    if (hp::overflow::make::Div(a, rest...) || hp::underflow::make::Div(a, rest...)) {
                        return std::nullopt;
                        }
                    return (a / ... / rest);
                    }
                } // namespace make
            } // namespace Try
        } // namespace safe
    } // namespace hp