#pragma once

#include "hp/flows/overflow.hpp"
#include "hp/flows/underflow.hpp"

namespace hp {

    namespace safe {

        // ----- Checks for Under and Overflow | Add
        template <typename T>
        constexpr bool Add(T a, T b) {
            static_assert(std::is_arithmetic_v<T>, "Arguments must be arithmetic");
            if (hp::overflow::check(a, b) && hp::underflow::check(a, b)) {
                return true;
                }
            return false;
            }

        // ----- Check for Under and Overflow | Sub
        template <typename T>
        constexpr bool Sub(T a, T b) {
            static_assert
            }
        }
    }