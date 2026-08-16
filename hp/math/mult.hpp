#pragma once
#include <type_traits>
#include <string>
#include <sstream>
#include <stdexcept>
#include "hp/other/overflow.hpp"
#include "hp/other/underflow.hpp"

namespace hp {

    // ----- Simple mult for two arguments
    template <typename T>
    [[nodiscard]] T mult(T a, T b) {
        if (hp::overflow::check::Mult(a, b)) {
            throw std::overflow_error(
                std::string("Overflow  in ") + __FUNCTION__ +
                " inside " + __FILE__ + " at line: " +
                std::to_string(__LINE__));
            }
        else if (hp::underflow::check::Mult(a, b)) {
            throw std::underflow_error(
                std::string("Underflow  in ") + __FUNCTION__ +
                " inside " + __FILE__ + " at line: " +
                std::to_string(__LINE__));
            }
        return a * b;
        }

    // ----- Mult multiple arguments (parameter pack)
    template <typename... T>
    [[nodiscard]] auto mult(T... args) {
        static_assert((std::is_arithmetic_v<T> && ...),
            "All arguments must be numbers!");
        if (((args != 0) && ...)) {
            if (hp::overflow::make::Mult(args...)) {
                throw std::overflow_error(
                    std::string("Overflow  in ") + __FUNCTION__ +
                    " inside " + __FILE__ + " at line: " +
                    std::to_string(__LINE__));
                }
            else if (hp::underflow::make::Mult(args...)) {
                throw std::underflow_error(
                    std::string("Underflow  in ") + __FUNCTION__ +
                    " inside " + __FILE__ + " at line: " +
                    std::to_string(__LINE__));
                }
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
                case '*':
                    if (hp::overflow::check::Mult(result, next)) {
                        throw std::overflow_error(
                            std::string("Overflow  in ") + __FUNCTION__ +
                            " inside " + __FILE__ + " at line: " +
                            std::to_string(__LINE__));
                        }
                    else if (hp::underflow::check::Mult(result, next)) {
                        throw std::underflow_error(
                            std::string("Underflow  in ") + __FUNCTION__ +
                            " inside " + __FILE__ + " at line: " +
                            std::to_string(__LINE__));
                        }
                    result *= next; break;
                default:
                    throw std::runtime_error("Only '*' supported in hp::mult");
                }
            }
        return result;
        }

    }