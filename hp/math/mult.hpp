#pragma once
#include <type_traits>
#include <string>
#include <sstream>
#include <stdexcept>

namespace hp {

    // ----- Simple mult for two arguments
    template <typename T>
    [[nodiscard]] T mult(T a, T b) {
        return a * b;
        }

    // ----- Mult multiple arguments (parameter pack)
    template <typename... T>
    [[nodiscard]] auto mult(T... args) {
        static_assert((std::is_arithmetic_v<T> && ...),
            "All arguments must be numbers!");
        if (((args != 0) && ...)) {
            return (args * ...);
            }
        else {
            throw std::runtime_error("All arguments must be non-zero!");
            }
        }

    // ----- Mult from string (parses expression)
    template <typename T = double>
    [[nodiscard]] T mult(const std::string& expression) {
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic!");
        std::stringstream ss(expression);

        T result;
        ss >> result;

        char op;
        T next;
        while (ss >> op >> next) {
            switch (op) {
                case '*': result *= next; break;
                default:
                    throw std::runtime_error("Only '*' supported in hp::mult");
                }
            }
        return result;
        }

    }