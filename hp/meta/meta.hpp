#pragma once

#include <meta>
#include <span>
#include <string_view>

namespace hp::meta {

    template <typename T, typename U>
    consteval auto same_as() noexcept -> bool {
        return (^^T == ^^U);
    }

} // namespace hp::meta