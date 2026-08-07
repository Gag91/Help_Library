#pragma once
#include <type_traits>
#include <string>
#include <sstream>
#include <stdexcept>

namespace hp {

    // ----- Simple div for two arguments
    template <typename T>
    [[nodiscard]] T div(T a, T b) {
        if (b == 0) throw std::runtime_error("Cannot divide by 0");
        return a / b;
        }

    // ----- Div multiple arguments (parameter pack)
    template <typename... T>
    [[nodiscard]] auto div(T... args) {
        static_assert((std::is_arithmetic_v<T> && ...),
            "All arguments must be numbers!");
        return (args / ...);
        }

    // ----- Div from string (parses expression)
    template <typename T = double>
    [[nodiscard]] T div(const std::string& expression) {
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic!");
        std::stringstream ss(expression);

        T result;
        ss >> result;

        char op;
        T next;
        while (ss >> op >> next) {
            switch (op) {
                case '/':
                    if (next == 0) throw std::runtime_error("Cannot divide by 0");
                    result /= next;
                    break;
                default:
                    throw std::runtime_error("Only '/' supported in hp::div");
                }
            }
        return result;
        }

    }