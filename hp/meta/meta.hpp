#pragma once

#include <meta>
#include <span>
#include <string_view>
#include <type_traits>

consteval std::meta::info typeof_impl(std::meta::info r) {
    return std::meta::is_type(r) ? std::meta::decay(r) : std::meta::decay(std::meta::type_of(r));
}

#define typeof(X) typename[:typeof_impl(^^X):]

namespace hp::meta {

    template <typename T, typename U>
    consteval auto same_as() noexcept -> bool {
        return (^^T == ^^U);
    }

} // namespace hp::meta