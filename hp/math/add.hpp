#pragma once
#include <type_traits>
#include <string>
#include <sstream>
#include <stdexcept>

namespace hp {

    // ----- Simple add for two arguments
    template <typename T>
    [[nodiscard]] T add(T a, T b) {
        return a + b;
        }

    // ----- Add multiple arguments (parameter pack)
    template <typename... T>
    [[nodiscard]] auto add(T... args) {
        static_assert((std::is_arithmetic_v<T> && ...),
            "All arguments must be numbers!");
        return (args + ...);
        }

    // ----- Add from string (parses expression)
    template <typename T = double>
    [[nodiscard]] T add(const std::string& expression) {
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic!");
        std::stringstream ss(expression);

        T result;
        ss >> result;

        char op;
        T next;
        while (ss >> op >> next) {
            switch (op) {
                case '+': result += next; break;
                default:
                    throw std::runtime_error("Only '+' supported in hp::add");
                }
            }
        return result;
        }

    }