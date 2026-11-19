
#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace hp {

    template <typename... Ts>
    struct type_list;

    template <typename... Ts>
    struct push_back;

    template <typename... Ts>
    struct push_front;

    template <typename U, typename... Ts>
    struct pop_front_impl;

    template <typename U, typename... Ts>
    struct pop_back_impl;

    template <typename U, typename T>
    struct erase_impl;

    template <typename T, typename... Ts>
    struct prepend;

    template <std::size_t N, typename... Ts>
    struct at_impl;

    template <typename... Ts>
    struct at;

    template <typename... Ts>
    struct indexOf;

    template <template <typename> typename Predicate, typename... Ts>
    struct filter_impl;

    template <typename... Ts>
    struct concat_impl;

    template <typename... Ts>
    struct concat;

    template <typename U, typename... Ts>
    struct apply_impl;

    template <std::size_t Start, std::size_t End, typename... Ts>
    struct slice_impl;

    template <std::size_t Start, std::size_t End, typename... Ts>
    struct slice;

    template <typename... Ts>
    struct sort;

    template <typename Ts>
    struct sort_impl;

    template <std::size_t N, typename T, typename List>
    struct replace_impl;

    template <std::size_t N, typename T, typename... Ts>
    struct replace_impl<N, T, type_list<Ts...>> {
        static_assert(N < sizeof...(Ts), "replace: index out of range");

        using type = typename type_list<Ts...>::template slice<0, N>::template append<T>::template append_list<typename type_list<Ts...>::template slice<N + 1, sizeof...(Ts)>>;
    };

    template <std::size_t N, typename T, typename List>
    using replace = typename replace_impl<N, T, List>::type;

    template <std::size_t N, typename T>
    struct repeat_impl;

    template <std::size_t N, typename T>
    struct repeat_impl {
        using type = typename prepend<T, typename repeat_impl<N - 1, T>::type>::type;
    };

    template <typename T>
    struct repeat_impl<0, T> {
        using type = type_list<>;
    };

    template <typename T, std::size_t N>
    using repeat = typename repeat_impl<N, T>::type;

    template <typename... Ts>
    struct sort_impl<type_list<Ts...>> {
      private:
        template <std::size_t... Is>
        static constexpr auto make_arr(std::index_sequence<Is...>) {
            return std::array<std::pair<std::size_t, std::size_t>, sizeof...(Ts)>{
                std::pair<std::size_t, std::size_t>{sizeof(Ts), Is}...};
        }

        static constexpr auto sorted = [] {
            auto arr = make_arr(std::make_index_sequence<sizeof...(Ts)>{});
            std::sort(arr.begin(), arr.end(), [](const auto &a, const auto &b) {
                return a.first < b.first || (a.first == b.first && a.second < b.second);
            });
            return arr;
        }();

        template <std::size_t... Is>
        static auto build(std::index_sequence<Is...>) -> type_list<std::tuple_element_t<sorted[Is].second, std::tuple<Ts...>>...>;

      public:
        using type = decltype(build(std::make_index_sequence<sizeof...(Ts)>{}));
    };

    template <typename T, typename... Ts>
    struct prepend<T, type_list<Ts...>> {
        using type = type_list<T, Ts...>;
    };

    template <template <typename> typename Predicate>
    struct filter_impl<Predicate> {
        using type = type_list<>;
    };

    template <template <typename> typename Predicate, typename T, typename... Ts>
    struct filter_impl<Predicate, T, Ts...> {
        using tail = typename filter_impl<Predicate, Ts...>::type;
        using type = std::conditional_t<Predicate<T>::value, typename prepend<T, tail>::type, tail>;
    };

    template <std::size_t N, typename... Ts>
    struct at_impl {
        static_assert(N < sizeof...(Ts), "type_list index out of bounds");
        using type = std::tuple_element_t<N, std::tuple<Ts...>>;
    };

    template <typename T>
    struct front_impl;

    template <typename T, typename... Ts>
    struct front_impl<type_list<T, Ts...>> {
        using type = T;
    };

    template <>
    struct front_impl<type_list<>> {
        using type = void;
    };

    template <typename T>
    struct back_impl;

    template <typename T, typename... Ts>
    struct back_impl<type_list<T, Ts...>> {
        using type = std::tuple_element_t<sizeof...(Ts), std::tuple<T, Ts...>>;
    };

    template <>
    struct back_impl<type_list<>> {
        using type = void;
    };

    template <typename U>
    struct not_same_as {
        template <typename T>
        using apply = std::bool_constant<!std::is_same_v<U, T>>;
    };

    template <typename U, typename... Ts>
    struct erase_impl<U, type_list<Ts...>> {
        using type = typename type_list<Ts...>::template filter<not_same_as<U>::template apply>;
    };

    template <typename U>
    struct pop_front_impl<type_list<U>> {
        using type = type_list<>;
    };

    template <>
    struct pop_front_impl<type_list<>> {
        using type = type_list<>;
    };

    template <typename U, typename... Ts>
    struct pop_front_impl<type_list<U, Ts...>> {
        using type = type_list<Ts...>;
    };

    template <>
    struct pop_back_impl<type_list<>> {
        using type = type_list<>;
    };

    template <typename U>
    struct pop_back_impl<type_list<U>> {
        using type = type_list<>;
    };

    template <typename U, typename V, typename... Ts>
    struct pop_back_impl<type_list<U, V, Ts...>> {
        using type = typename prepend<U, typename pop_back_impl<type_list<V, Ts...>>::type>::type;
    };

    template <typename... Ts>
    struct concat_impl {
        using type = type_list<Ts...>;
    };

    template <typename... Ts, typename... Us>
    struct concat<type_list<Ts...>, type_list<Us...>> {
        using type = typename concat_impl<Ts..., Us...>::type;
    };

    template <std::size_t Start, std::size_t End, typename... Ts>
    struct slice_impl<Start, End, type_list<Ts...>> {
        static_assert(Start <= End, "slice start must be <= end");
        static_assert(End <= sizeof...(Ts), "slice end out of range");

        template <std::size_t... Is>
        static auto make_type_list(std::index_sequence<Is...>)
            -> type_list<typename std::tuple_element_t<Start + Is, std::tuple<Ts...>>...>;

        using type = decltype(make_type_list(std::make_index_sequence<End - Start>{}));
    };

    template <std::size_t Start, std::size_t End, typename... Ts>
    struct slice<Start, End, type_list<Ts...>> {
        using type = typename slice_impl<Start, End, type_list<Ts...>>::type;
    };

    template <typename T>
    struct is_type_list : std::false_type {};
    template <typename... Ts>
    struct is_type_list<type_list<Ts...>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_type_list_v = is_type_list<T>::value;

    template <typename... Ts>
    struct type_list {

        template <typename T>
        using append = typename concat<type_list<Ts...>, type_list<T>>::type;

        template <typename U>
        using append_list = typename concat<type_list<Ts...>, U>::type;

        template <std::size_t Start, std::size_t End>
        using slice = typename slice<Start, End, type_list<Ts...>>::type;

        template <typename... Us>
        using push_back = type_list<Ts..., Us...>;

        template <typename... Us>
        using push_front = type_list<Us..., Ts...>;

        template <template <typename> typename Predicate>
        using filter = typename filter_impl<Predicate, Ts...>::type;

        template <std::size_t N>
        using at = typename at_impl<N, Ts...>::type;

        template <typename U>
        using erase = typename erase_impl<U, type_list<Ts...>>::type;

        using pop_front = typename pop_front_impl<type_list<Ts...>>::type;
        using pop_back = typename pop_back_impl<type_list<Ts...>>::type;
        using front = typename front_impl<type_list<Ts...>>::type;
        using back = typename back_impl<type_list<Ts...>>::type;
        using sort = typename sort_impl<type_list<Ts...>>::type;
        using clear = type_list<>;
        using to_tuple = std::tuple<Ts...>;

        static constexpr std::size_t size = sizeof...(Ts);
        static constexpr bool empty = (sizeof...(Ts) == 0);

        template <template <typename> typename Pred>
        static constexpr bool all_of = (Pred<Ts>::value && ...);

        template <template <typename> typename Pred>
        static constexpr bool any_of = (Pred<Ts>::value || ...);

        template <typename U>
        static constexpr bool contains = (std::is_same_v<U, Ts> || ...);

        template <typename T>
            requires(contains<T>)
        static constexpr std::size_t indexOf = ([]() -> std::size_t {
            std::size_t i = 0;
            ((std::is_same_v<Ts, T> ? false : ++i) && ...);
            return i;
        }());

        template <typename T>
        static constexpr std::size_t count = ([]() -> std::size_t {
            if (!contains<T>)
                return 0;
            std::size_t i = 0;
            ((std::is_same_v<Ts, T> ? ++i : 0), ...);
            return i;
        }());

        template <template <typename> typename Pred>
        static constexpr std::size_t count_if = ([]() -> std::size_t {
            std::size_t i = 0;
            ((Pred<Ts>::value ? ++i : 0), ...);
            return i;
        }());
        ;

        template <typename Predicate>
        static void for_each(Predicate &&func) {
            (func(Ts{}), ...);
        }
    };

}; // namespace hp