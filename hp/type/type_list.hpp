#pragma once

#include "hp/meta/types.hpp"

#include <array>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace hp {

    template <typename... Ts>
    struct type_list;

    template <typename List>
    struct front_impl;

    template <typename List>
    struct Tail;

    template <typename List>
    struct back_impl;

    template <typename List>
    struct clear_impl;

    template <typename H, typename... T>
    struct Tail<type_list<H, T...>> {
        using type = type_list<T...>;
    };

    template <typename T, typename... Us>
    struct front_impl<type_list<T, Us...>> {
        using type = T;
    };

    template <typename T>
    struct back_impl<type_list<T>> {
        using type = T;
    };

    template <typename H, typename... T>
    struct back_impl<type_list<H, T...>> {
        using type =
            typename back_impl<typename Tail<type_list<H, T...>>::type>::type;
    };

    template <typename... Ts>
    struct clear_impl<type_list<Ts...>> {
        using type = type_list<>;
    };

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

    template <typename>
    struct pop_back_impl {};

    template <typename T, typename... Ts>
    struct pop_back_impl<type_list<T, Ts...>> {
        using type = type_list<Ts...>;
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

        template <typename U>
        using push_back = type_list<Ts..., U>;

        template <typename U>
        using push_front = type_list<U, Ts...>;

        using clear = typename clear_impl<type_list<Ts...>>::type;
        using front = typename front_impl<type_list<Ts...>>::type;
        using back = typename back_impl<type_list<Ts...>>::type;
        using pop_front = typename pop_front_impl<type_list<Ts...>>::type;

        template <std::size_t index>
            requires(index < sizeof...(Ts))
        using get = Ts...[index];

        static constexpr std::size_t size = sizeof...(Ts);
        static constexpr bool empty = (sizeof...(Ts) == 0);

        template <typename U>
        static constexpr bool contains = (std::is_same_v<U, Ts> || ...);

        template <typename Predicate>
        static void for_each(Predicate &func) {
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

        template <typename U>
        using push_back = type_list<U>;

        template <typename U>
        using push_front = type_list<U>;
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
