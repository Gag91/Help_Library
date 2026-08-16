#pragma once
#include <type_traits>
#include <string>
#include <sstream>
#include <stdexcept>
#include "hp/other/overflow.hpp"
#include "hp/other/underflow.hpp"

namespace hp {

    // ----- Simple sub for two arguments
    template <typename T>
    [[nodiscard]] T sub(T a, T b) {
        if (hp::overflow::check::Sub(a, b)) {
            throw std::overflow_error(
                std::string("Overflow  in ") + __FUNCTION__ +
                " inside " + __FILE__ + " at line: " +
                std::to_string(__LINE__));
            }
        else if (hp::underflow::check::Sub(a, b)) {
            throw std::underflow_error(
                std::string("Underflow  in ") + __FUNCTION__ +
                " inside " + __FILE__ + " at line: " +
                std::to_string(__LINE__));
            }
        return a - b;
        }

    // ----- Sub multiple arguments (parameter pack)
    template <typename... T>
    [[nodiscard]] auto sub(T... args) {
        static_assert((std::is_arithmetic_v<T> && ...),
            "All arguments must be numbers!");
        if (hp::overflow::make::Sub(args...)) {
            throw std::overflow_error(
                std::string("Overflow  in ") + __FUNCTION__ +
                " inside " + __FILE__ + " at line: " +
                std::to_string(__LINE__));
            }
        else if (hp::underflow::make::Sub(args...)) {
            throw std::underflow_error(
                std::string("Underflow  in ") + __FUNCTION__ +
                " inside " + __FILE__ + " at line: " +
                std::to_string(__LINE__));
            }
        return (args - ...);
        }

    // ----- Sub from string (parses expression)
    template <typename T = double>
    [[nodiscard]] T sub(const std::string& expression) {
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic!");
        std::stringstream ss(expression);

        T result;
        ss >> result;

        char op;
        T next;
        while (ss >> op >> next) {
            switch (op) {
                case '-':
                    if (hp::overflow::check::Sub(result, next)) {
                        throw std::overflow_error(
                            std::string("Overflow  in ") + __FUNCTION__ +
                            " inside " + __FILE__ + " at line: " +
                            std::to_string(__LINE__));
                        }
                    else if (hp::underflow::check::Sub(result, next)) {
                        throw std::underflow_error(
                            std::string("Underflow  in ") + __FUNCTION__ +
                            " inside " + __FILE__ + " at line: " +
                            std::to_string(__LINE__));
                        }
                    result -= next; break;
                default:
                    throw std::runtime_error("Only '-' supported in hp::sub");
                }
            }
        return result;
        }

    }