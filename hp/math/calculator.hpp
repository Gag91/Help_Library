#pragma once
#include <type_traits>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

namespace hp {

    // ----- Simple calculator (two operands)
    template <typename T>
    [[nodiscard]] T calc(const std::string& expression) {
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic!");
        std::stringstream ss(expression);
        T a, b;
        char op;
        ss >> a >> op >> b;
        if (ss.fail()) throw std::runtime_error("Invalid Operation");

        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/':
                if (b == 0) throw std::runtime_error("Cannot divide by 0");
                return a / b;
            case '%':
                if constexpr (std::is_integral_v<T>) {
                    if (b == 0) throw std::runtime_error("Cannot modulo by 0");
                    return a % b;
                    }
                else {
                    throw std::runtime_error("Modulo (%) only works for integers! Use floating point division.");
                    }
            case '^':
                if constexpr (std::is_same_v<T, double>) {
                    return std::pow(a, b);
                    }
                else {
                    throw std::runtime_error("Operator '^' only works for double");
                    }
            default:
                throw std::runtime_error("Invalid Operation");
            }
        }

    // ----- Advanced calculator (multiple operands, left-to-right)
    template <typename T = double>
    [[nodiscard]] T calcAdvanced(const std::string& expression) {
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic!");
        std::stringstream ss(expression);

        T result;
        ss >> result;

        char op;
        T next;
        while (ss >> op >> next) {
            switch (op) {
                case '+': result += next; break;
                case '-': result -= next; break;
                case '*': result *= next; break;
                case '/':
                    if (next == 0) throw std::runtime_error("Cannot divide by 0");
                    result /= next;
                    break;
                case '%':
                    if constexpr (std::is_integral_v<T>) {
                        if (next == 0) throw std::runtime_error("Cannot modulo by 0");
                        result %= next;
                        }
                    else {
                        throw std::runtime_error("Modulo (%) only works for integers!");
                        }
                    break;
                case '^':
                    if constexpr (std::is_same_v<T, double>) {
                        result = std::pow(result, next);
                        }
                    else {
                        throw std::runtime_error("Operator '^' only works for double");
                        }
                    break;
                default:
                    throw std::runtime_error("Invalid Operation");
                }
            }
        return result;
        }

    }