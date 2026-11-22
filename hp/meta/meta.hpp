#pragma once

#include "hp/other/type_traits.hpp"
#include "hp/string/string.hpp"

#include <array>
#include <meta>
#include <span>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

// ----- typeof(x) implementation
consteval std::meta::info typeof_impl(std::meta::info r) {
    return std::meta::is_type(r) ? std::meta::decay(r) : std::meta::decay(std::meta::type_of(r));
}

#define typeof(X) typename[:typeof_impl(^^X):]

namespace hp::meta {

    // ----- Check if two types are the same
    template <typename T, typename U>
    consteval auto same_as() noexcept -> bool {
        return (^^T == ^^U);
    }

    // ----- Get enumerators of an enum
    template <typename T>
        requires std::is_enum_v<T>
    [[nodiscard]] constexpr auto get_enumerators() noexcept {
        static constexpr auto enums = std::define_static_array(std::meta::enumerators_of(^^T));
        std::array<T, enums.size()> arr{};
        std::size_t i = 0;

        template for (constexpr auto e : enums) {
            arr[i++] = [:e:];
        }
        return arr;
    }

    // ----- Reflected info and value for a class member
    template <std::meta::info M, typename MemberType>
    struct member_info {
        static constexpr std::meta::info meta = M;
        static constexpr std::meta::info type = std::meta::type_of(M);

        static constexpr std::string_view identifier = std::meta::identifier_of(M);
        static constexpr std::string_view type_name = std::meta::display_string_of(type);
        static constexpr std::size_t offset = std::meta::offset_of(M).bytes;
        static constexpr std::size_t size = std::meta::size_of(M);
        static constexpr std::size_t alignment = std::meta::alignment_of(M);

        static constexpr bool is_const = std::meta::is_const_type(type);
        static constexpr bool is_pointer = std::meta::is_pointer_type(type);
        static constexpr bool is_reference = std::meta::is_reference_type(type);
        static constexpr bool is_class = std::meta::is_class_type(type);
        static constexpr bool is_enum = std::meta::is_enum_type(type);
        static constexpr bool is_arithmetic = std::meta::is_arithmetic_type(type);

        static constexpr bool is_public = std::meta::is_public(M);
        static constexpr bool is_private = std::meta::is_private(M);
        static constexpr bool is_protected = std::meta::is_protected(M);

        MemberType &value;
    };

    // ----- Get a tuple of member_info for all non-static data members
    template <typename T>
    [[nodiscard]] constexpr auto get_members(T &obj) {
        static constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));

        return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            return std::make_tuple(
                member_info<members[Is], decltype(obj.[:members[Is]:])>{
                    obj.[:members[Is]:]}...);
        }(std::make_index_sequence<members.size()>{});
    }

    // ----- Do x on each non-static data member
    template <typename T, typename Fn>
    constexpr void for_each_member(T &obj, Fn &&fn) {
        template for (auto m : get_members(obj)) {
            fn(m);
        }
    }

    // ----- Compare all non-static data members of two objects
    template <typename T>
    [[nodiscard]] constexpr bool equals(const T &lhs, const T &rhs) {
        static constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));

        template for (constexpr auto m : members) {
            if (lhs.[:m:] != rhs.[:m:]) {
                return false;
            }
        }
        return true;
    }

    // ----- Get the display name of a type
    template <typename T>
    [[nodiscard]] consteval std::string_view type_name() noexcept {
        return std::meta::display_string_of(^^T);
    }

    // ----- Get the number of non-static data members
    template <typename T>
    [[nodiscard]] consteval std::size_t member_count() noexcept {
        return std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()).size();
    }

    // ----- Get the identifier name of an enum value
    template <typename E>
        requires std::is_enum_v<E>
    [[nodiscard]] constexpr std::string_view enumerator_name(E value) noexcept {
        static constexpr auto enums = std::define_static_array(std::meta::enumerators_of(^^E));
        template for (constexpr auto e : enums) {
            if (value == [:e:]) {
                return std::meta::identifier_of(e);
            }
        }
        return {};
    }

} // namespace hp::meta