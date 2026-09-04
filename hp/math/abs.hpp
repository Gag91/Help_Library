#pragma once

#include "hp/math/make.hpp"

namespace hp
{

    // ----- Absolute value
    template <typename T>
    [[nodiscard]] T abs(T a)
    {
        return hp::make::positive(a);
    }
} // namespace hp