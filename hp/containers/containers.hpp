#pragma once

#include <iostream>
#include <vector>
#include <ranges>
#include <string>
#include <algorithm>

namespace hp {

    // ----- Contains for string (specific overload) -----
    bool contains(const std::string& str, const std::string& substr) {
        return str.find(substr) != std::string::npos;
        }

    // ----- Contains for string + const char* (to avoid ambiguity) -----
    bool contains(const std::string& str, const char* substr) {
        return str.find(substr) != std::string::npos;
        }

    // ----- Contains for string + char -----
    bool contains(const std::string& str, char c) {
        return str.find(c) != std::string::npos;
        }

    // ----- Contains for containers (generic, single value) -----
    template<std::ranges::range T, typename U>
    bool contains(const T& container, const U& value) {
        for (const auto& elem : container) {
            if (elem == value) return true;
            }
        return false;
        }

    // ----- Contains Any (multiple values) -----
    template<std::ranges::range T, typename... U>
    bool containsAny(const T& container, const U&... values) {
        return (contains(container, values) || ...);
        }

    // ----- Contains All (multiple values) -----
    template<std::ranges::range T, typename... U>
    bool containsAll(const T& container, const U&... values) {
        return (contains(container, values) && ...);
        }

    // ----- Print All -----
    template<std::ranges::range T>
    void printAll(const T& containers) {
        for (size_t i = 0; i < containers.size(); i++) {
            std::cout << containers[i];
            }
        }

    // ----- Print All with newline -----
    template<std::ranges::range T>
    void printlnAll(const T& containers) {
        for (size_t i = 0; i < containers.size(); i++) {
            std::cout << containers[i] << std::endl;
            }
        }

    // ----- Sum All (numbers only) -----
    template<std::ranges::range T>
        requires std::is_arithmetic_v<std::ranges::range_value_t<T>>
    auto sumAll(const T& containers) {
        using etype = std::ranges::range_value_t<T>;
        etype result = 0;
        for (const auto& elem : containers) {
            result += elem;
            }
        return result;
        }

    // ----- Index Of an Item -----
    template<std::ranges::range Container, typename T>
    int indexOf(const Container& c, const T& value) {
        using etype = std::ranges::range_value_t<Container>;
        static_assert(std::is_same_v<etype, T>, "Value type must match container element type!");
        int index = 0;
        for (const auto& elem : c) {
            if (elem == value) return index;
            index++;
            }
        return -1;
        }

    // ----- Append to vector -----
    template <typename T, typename... Args>
    void append(std::vector<T>& vec, Args... rest) {
        (vec.push_back(rest), ...);
        }

    }