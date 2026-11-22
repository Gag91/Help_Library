#pragma once

#include "hp/colors/color.hpp"
#include "hp/meta/meta.hpp"

#include <iostream>
#include <meta>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <vector>

class Random {
  private:
    // ----- Generator
    static inline std::random_device rd;
    static inline std::mt19937 gen{rd()};

  public:
    // ---- Generate a random number
    template <typename T = std::size_t>
    static T rand(T min, T max) {
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(gen);
        } else {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(gen);
        }
    }

    // ----- Pick a random item from a vector
    template <typename T>
    static T choice(const std::vector<T> &vec) {
        if (vec.empty()) {
            throw std::runtime_error("Cannot pick from empty vector!");
        }
        int index = rand<int>(0, vec.size() - 1);
        return vec[index];
    }

    // ----- Pick a random item from a list
    template <typename T>
    static T choice(std::initializer_list<T> list) {
        if (list.size() == 0) {
            throw std::runtime_error("Cannot pick from empty list!");
        }
        int index = rand<int>(0, list.size() - 1);
        auto it = list.begin();
        std::advance(it, index);
        return *it;
    }

    // ----- Shuffle randomly a vector
    template <typename T>
    static void shuffle(std::vector<T> &vec) {
        for (int i = vec.size() - 1; i > 0; i--) {
            int j = rand<int>(0, i);
            std::swap(vec[i], vec[j]);
        }
    }

    // ----- Pick a random value from an enum
    template <typename T>
        requires std::is_enum_v<T>
    [[nodiscard]] static T randomValue() {
        static constexpr auto values = get_enumerators<T>();
        static_assert(!values.empty(), "Cannot pick a random value from an empty enum!");
        return values[rand<std::size_t>(0, values.size() - 1)];
    }

    [[nodiscard]] static hp::Color randomColor() {
        return randomValue<hp::Color>();
    }
};