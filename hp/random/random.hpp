#pragma once

#include <iostream>
#include <random>
#include <vector>
#include <type_traits>  
#include <stdexcept>    
#include "hp/colors/color.hpp"


    // ----- Main Class
    class Random {
    private:

        // ----- Generator
        static std::random_device rd;
        static std::mt19937 gen;

    public:

        // ---- Generate a random number
        template<typename T>
        static T rand(T min, T max) {
            if constexpr (std::is_integral<T>::value) {
                std::uniform_int_distribution<T> dist(min, max);
                return dist(gen);
                }
            else {
                std::uniform_real_distribution<T> dist(min, max);
                return dist(gen);
                }
            }

        // Pick a random item from a vector
        template<typename T>
        static T choice(const std::vector<T>& vec) {
            if (vec.empty()) {
                throw std::runtime_error("Cannot pick from empty vector!");
                }
            int index = rand<int>(0, vec.size() - 1);
            return vec[index];
            }

        // Pick a random item from a list
        template<typename T>
        static T choice(std::initializer_list<T> list) {
            if (list.size() == 0) {
                throw std::runtime_error("Cannot pick from empty list!");
                }
            int index = rand<int>(0, list.size() - 1);
            auto it = list.begin();
            std::advance(it, index);
            return *it;
            }

        // Shuffle randomly a vector
        template<typename T>
        static void shuffle(std::vector<T>& vec) {
            for (int i = vec.size() - 1; i > 0; i--) {
                int j = rand<int>(0, i);
                std::swap(vec[i], vec[j]);
                }
            }
        [[nodiscard]] inline hp::Color randomColor() {
            int index = Random::rand(0, hp::COLOR_COUNT - 1);
            return static_cast<hp::Color>(index);
            }
        };
    std::random_device Random::rd;
    std::mt19937 Random::gen(Random::rd());
    
