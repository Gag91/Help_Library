#pragma once

#include <algorithm>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

namespace hp {

    // ----- Contains for string (specific overload) -----
    inline bool contains(const std::string &str, const std::string &substr) {
        return str.find(substr) != std::string::npos;
    }

    // ----- Contains for string + const char* (to avoid ambiguity) -----
    inline bool contains(const std::string &str, const char *substr) {
        return str.find(substr) != std::string::npos;
    }

    // ----- Contains for string + char -----
    inline bool contains(const std::string &str, char c) {
        return str.find(c) != std::string::npos;
    }

    // ----- Contains for containers (generic, single value) -----
    template <std::ranges::range T, typename U>
    inline bool contains(const T &container, const U &value) {
        for (const auto &elem : container) {
            if (elem == value)
                return true;
        }
        return false;
    }

    // ----- Contains Any (multiple values) -----
    template <std::ranges::range T, typename... U>
    inline bool containsAny(const T &container, const U &...values) {
        return (contains(container, values) || ...);
    }

    // ----- Contains All (multiple values) -----
    template <std::ranges::range T, typename... U>
    inline bool containsAll(const T &container, const U &...values) {
        return (contains(container, values) && ...);
    }

    // ----- Print All -----
    template <std::ranges::range T>
    inline void printAll(const T &containers) {
        for (size_t i = 0; i < containers.size(); i++) {
            std::cout << containers[i];
        }
    }

    // ----- Print All with newline -----
    template <std::ranges::range T>
    inline void printlnAll(const T &containers) {
        for (size_t i = 0; i < containers.size(); i++) {
            std::cout << containers[i] << std::endl;
        }
    }

    // ----- Sum All (numbers only) -----
    template <std::ranges::range T>
        requires std::is_arithmetic_v<std::ranges::range_value_t<T>>
    inline auto sumAll(const T &containers) {
        using etype = std::ranges::range_value_t<T>;
        etype result = 0;
        for (const auto &elem : containers) {
            result += elem;
        }
        return result;
    }

    // ----- Index Of an Item -----
    template <std::ranges::range Container, typename T>
    inline int indexOf(const Container &c, const T &value) {
        using etype = std::ranges::range_value_t<Container>;
        static_assert(std::is_same_v<etype, T>, "Value type must match container element type!");
        int index = 0;
        for (const auto &elem : c) {
            if (elem == value)
                return index;
            index++;
        }
        return -1;
    }

    // ----- Append to vector -----
    template <typename T, typename... Args>
    inline void append(std::vector<T> &vec, Args... rest) {
        (vec.push_back(rest), ...);
    }

    // ----- Filter (keeps elements that satisfy predicate)
    template <std::ranges::range Container, typename Predicate>
    constexpr auto filter(const Container &container, Predicate pred) {
        using valueType = std::ranges::range_value_t<Container>;
        std::vector<valueType> result;
        for (const auto &elem : container) {
            if (pred(elem)) {
                result.push_back(elem);
            }
        }
        return result;
    }

    // ----- All elements satisfy predicate
    template <std::ranges::range Container, typename Predicate>
    inline bool allOf(const Container &container, Predicate pred) {
        for (const auto &elem : container) {
            if (!pred(elem)) {
                return false;
            }
        }
        return true;
    }

    // ----- Any element satisfies predicate
    template <std::ranges::range Container, typename Predicate>
    inline bool anyOf(const Container &container, Predicate pred) {
        for (const auto &elem : container) {
            if (pred(elem)) {
                return true;
            }
        }
        return false;
    }

    // ----- No elements satisfy predicate
    template <std::ranges::range Container, typename Predicate>
    inline bool noneOf(const Container &container, Predicate pred) {
        for (const auto &elem : container) {
            if (pred(elem)) {
                return false;
            }
        }
        return true;
    }

    // ----- Count elements that satisfy predicate
    template <std::ranges::range Container, typename Predicate>
    inline size_t countIf(const Container &container, Predicate pred) {
        size_t counter = 0;
        for (const auto &elem : container) {
            if (pred(elem)) {
                counter++;
            }
        }
        return counter;
    }

    // ----- Find first element that satisfies predicate
    template <std::ranges::range Container, typename Predicate>
    std::optional<std::ranges::range_value_t<Container>>
    findIf(const Container &container, Predicate pred) {
        for (const auto &elem : container) {
            if (pred(elem)) {
                return elem;
            }
        }
        return std::nullopt;
    }

    namespace remove {

        // ----- Removes Duplicate
        template <std::ranges::range Container>
        auto duplicates(Container &container) noexcept -> Container {
            using Elem = std::ranges::range_value_t<Container>;
            std::vector<Elem> seen;
            std::erase_if(container, [&](const Elem &v) {
                if (std::find(seen.begin(), seen.end(), v) != seen.end()) {
                    return true;
                }
                seen.push_back(v);
                return false;
            });
            return container;
        }

        // ----- Removes positives numbers
        template <std::ranges::range Container>
        auto positive(Container &container) noexcept -> Container {
            using Elem = std::ranges::range_value_t<Container>;
            static_assert(std::is_arithmetic_v<Elem>, "hm::removes::positive: Container element type must be arithmetic");
            std::erase_if(container, [&](Elem n) { return n > 0; });
            return container;
        }

        // ----- Removes negatives numbers
        template <std::ranges::range Container>
        auto negative(Container &container) noexcept -> Container {
            using Elem = std::ranges::range_value_t<Container>;
            static_assert(std::is_arithmetic_v<Elem>, "hm::removes::negative: Container element type must be arithmetic");
            std::erase_if(container, [&](Elem n) { return n < 0; });
            return container;
        }

    } // namespace remove

} // namespace hp