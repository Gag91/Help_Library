#pragma once

#include "hp/meta/types.hpp"

#include <concepts>
#include <iostream>
#include <type_traits>

namespace hp {

    template <typename>
    constexpr bool always_false = false;

    template <typename T>
    concept end = always_false<T>;

    template <typename... Ts>
    struct type_list;

    template <typename>
    struct pop_front_impl {};

    template <typename T, typename... Ts>
    struct pop_front_impl<type_list<T, Ts...>> {
        using type = type_list<Ts...>;
    };

    template <>
    struct pop_front_impl<type_list<>> {
        using type = type_list<>;
    };

    template <typename List, typename T>
    struct prepend_to;

    template <typename... Ts, typename T>
    struct prepend_to<type_list<Ts...>, T> {
        using type = type_list<T, Ts...>;
    };

    template <typename>
    struct pop_back_impl {};

    template <typename T>
    struct pop_back_impl<type_list<T>> {
        using type = type_list<>;
    };

    template <typename U, typename... Ts>
    struct pop_back_impl<type_list<U, Ts...>> {
        using rest = typename pop_back_impl<type_list<Ts...>>::type;
        using type = typename prepend_to<rest, U>::type;
    };

    template <>
    struct pop_back_impl<type_list<>> {
        using type = type_list<>;
    };

    template <typename... Ts>
    struct type_list {
        template <typename... Us>
        using append = type_list<Ts..., Us...>;

        template <typename... Us>
        using prepend = type_list<Us..., Ts...>;

        using clear = type_list<>;

        template <end...>
        using front = Ts...[0];

        template <end...>
        using back = Ts...[sizeof...(Ts) - 1];

        using pop_front = typename pop_front_impl<type_list<Ts...>>::type;
        using pop_back = typename pop_back_impl<type_list<Ts...>>::type;

        template <std::size_t index>
            requires(index < sizeof...(Ts))
        using get = Ts...[index];

        static constexpr std::size_t size = sizeof...(Ts);
        static constexpr bool empty = (sizeof...(Ts) == 0);

        template <typename U>
        static constexpr bool contains = (std::is_same_v<U, Ts> || ...);

        template <typename Predicate>
        static void for_each(Predicate &&func) {
            (func(Ts{}), ...);
        }

        template <typename T>
            requires(contains<T>)
        static constexpr std::size_t indexOf = ([] -> std::size_t {
            std::size_t i = 0;
            ((std::is_same_v<Ts, T> ? false : ++i) && ...);
            return i;
        }());

        template <typename T>
            requires(contains<T>)
        static constexpr std::size_t count = ([] -> std::size_t {
            std::size_t i = 0;
            ((std::is_same_v<Ts, T> ? i++ : 0), ...);
            return i;
        }());
    };

    template <>
    struct type_list<> {
        using clear = type_list<>;

        static constexpr std::size_t size = 0;
        static constexpr bool empty = true;

        template <typename U>
        static constexpr bool contains = false;

        template <typename T>
            requires(contains<T>)
        static constexpr std::size_t indexOf = 0;

        template <typename T>
            requires(contains<T>)
        static constexpr std::size_t count = 0;

        template <typename... Us>
        using append = type_list<Us...>;

        template <typename... Us>
        using prepend = type_list<Us...>;
    };

    template <typename... Ts>
    std::ostream &operator<<(std::ostream &os, const type_list<Ts...> &) {
        os << "type_list<";
        bool first = true;
        ((os << (first ? "" : ", ") << hp::type_of<Ts>(), first = false), ...);
        return os << ">";
    }

    template <typename T>
    inline constexpr T print_v{};

} // namespace hp