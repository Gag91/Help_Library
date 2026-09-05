#pragma once

#include <concepts>
#include <type_traits>
#include "hp/flows/overflow.hpp"
#include "hp/flows/underflow.hpp"
#include <iostream>
#include <ranges>
#include <vector>
#include <algorithm>



namespace hp {

    // ---- Int or Float
    template <typename T>
    concept is_intOrFloat =
        std::integral<T> || std::is_floating_point_v<T>;

    // ----- Number
    template <typename T>
    concept Number =
        std::is_arithmetic_v<T>;

    // ----- Integer
    template <typename T>
    concept Integer =
        std::integral<T>;

    template <typename T>
    concept SignedInteger =
        std::signed_integral<T>;

    template <typename T>
    concept UnsignedInteger =
        std::unsigned_integral<T>;

    template <typename T>
    concept Float =
        std::is_floating_point_v<T>;

    // ----- Addition
    template <typename T, typename U = T>
    concept Addable =
        requires(T t, U u) {
        t + u;
        { hp::overflow::check::Add(t, u) } -> std::convertible_to<bool>;
        };

    // ----- Subtraction
    template <typename T, typename U = T>
    concept Substractable =
        requires(T t, U u) {
        t - u;
        { hp::underflow::check::Sub(t, u) } -> std::convertible_to<bool>;
        };

    // ----- Multiplication
    template <typename T, typename U = T>
    concept Multipliable = requires(T t, U u) {
        t* u;
        { hp::overflow::check::Mult(t, u) } -> std::convertible_to<bool>;
        };

    // ----- Division
    template <typename T, typename U = T>
    concept Divisible = requires(T t, U u) {
        t / u;
        { hp::overflow::check::Div(t, u) } -> std::convertible_to<bool>;
        };

    // ----- Modulo (only for integral types)
    template <typename T, typename U = T>
    concept Moduloable = requires(T t, U u) {
        requires std::integral<T>;
        requires std::integral<U>;
    { hp::overflow::check::Div(t, u) } -> std::convertible_to<bool>;
        };

    // ----- Arithmetic (all operations)
    template <typename T>
    concept Arithmetic =
        Addable<T> && Substractable<T> &&
        Multipliable<T> && Divisible<T>;

    // ----- Safe Arithmetic (with overflow/underflow checks)
    template <typename T>
    concept SafeArithmetic =
        Arithmetic<T> &&
        requires(T a, T b) {
                { hp::overflow::check::Add(a, b) } -> std::convertible_to<bool>;
                { hp::underflow::check::Add(a, b) } -> std::convertible_to<bool>;
                { hp::overflow::check::Sub(a, b) } -> std::convertible_to<bool>;
                { hp::underflow::check::Sub(a, b) } -> std::convertible_to<bool>;
                { hp::overflow::check::Mult(a, b) } -> std::convertible_to<bool>;
                { hp::underflow::check::Mult(a, b) } -> std::convertible_to<bool>;
                { hp::overflow::check::Div(a, b) } -> std::convertible_to<bool>;
                { hp::underflow::check::Div(a, b) } -> std::convertible_to<bool>;
        };

    // ----- Comparable (less than)
    template <typename T, typename U = T>
    concept LessThanComparable =
        requires(T t, U u) {
                { t < u } -> std::convertible_to<bool>;
        };

    // ----- Comparable (equality)
    template <typename T, typename U = T>
    concept EqualityComparable =
        requires(T t, U u) {
                { t == u } -> std::convertible_to<bool>;
                { t != u } -> std::convertible_to<bool>;
        };

    // ----- Comparable (all comparisons)
    template <typename T>
    concept Comparable =
        EqualityComparable<T> &&
        LessThanComparable<T> &&
        requires(T a, T b) {
                { a > b } -> std::convertible_to<bool>;
                { a <= b } -> std::convertible_to<bool>;
                { a >= b } -> std::convertible_to<bool>;
        };

    // ----- Incrementable
    template <typename T>
    concept Incrementable =
        requires(T t) {
                { ++t } -> std::convertible_to<T&>;
                { t++ } -> std::convertible_to<T>;
        };

    // ----- Decrementable
    template <typename T>
    concept Decrementable =
        requires(T t) {
                { --t } -> std::convertible_to<T&>;
                { t-- } -> std::convertible_to<T>;
        };

    // ----- Negatable (minus value)
    template <typename T>
    concept Negatable =
        requires(T t) {
                { -t } -> std::convertible_to<T>;
        };

    // ----- Absolute value
    template <typename T>
    concept Absable =
        requires(T t) {
                { std::abs(t) } -> std::convertible_to<T>;
        };

    // ----- Square root
    template <typename T>
    concept Sqrtable =
        requires(T t) {
                { std::sqrt(t) } -> std::convertible_to<T>;
        };

    // ----- Power
    template <typename T, typename U = T>
    concept Powable =
        requires(T t, U u) {
                { std::pow(t, u) } -> std::convertible_to<T>;
        };

    // ----- Trigonometric
    template <typename T>
    concept Trigonometric =
        requires(T t) {
                { std::sin(t) } -> std::convertible_to<T>;
                { std::cos(t) } -> std::convertible_to<T>;
                { std::tan(t) } -> std::convertible_to<T>;
        };

    // ----- Min/Max
    template <typename T, typename U = T>
    concept MinMaxable =
        requires(T t, U u) {
                { std::min(t, u) } -> std::convertible_to<decltype(t + u)>;
                { std::max(t, u) } -> std::convertible_to<decltype(t + u)>;
        };

    // ----- String convertible
    template <typename T>
    concept Stringifiable =
        requires(T t) {
                { std::to_string(t) } -> std::convertible_to<std::string>;
        };

    // ----- Printable to cout
    template <typename T>
    concept Printable =
        requires(T t) {
                { std::cout << t } -> std::convertible_to<std::ostream&>;
        };

    // ----- Streamable (can read from cin)
    template <typename T>
    concept Streamable =
        requires(T t) {
                { std::cin >> t } -> std::convertible_to<std::istream&>;
        };

    // ----- Default constructible
    template <typename T>
    concept DefaultConstructible =
        std::default_initializable<T>;

    // ----- Copy constructible
    template <typename T>
    concept CopyConstructible =
        std::copy_constructible<T>;

    // ----- Move constructible
    template <typename T>
    concept MoveConstructible =
        std::move_constructible<T>;

    // ----- Swappable
    template <typename T>
    concept Swappable =
        requires(T & a, T & b) {
                { std::swap(a, b) } -> std::convertible_to<void>;
        };

    // ----- Hashable (for unordered containers)
    template <typename T>
    concept Hashable =
        requires(T t) {
                { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>;
        };

    // ----- Iterable (range-based for)
    template <typename T>
    concept Iterable =
        requires(T t) {
                { std::begin(t) } -> std::forward_iterator;
                { std::end(t) } -> std::sentinel_for<decltype(std::begin(t))>;
        };

    // ----- Container (size, empty, etc)
    template <typename T>
    concept Container =
        Iterable<T> &&
        requires(T t) {
                { t.size() } -> std::convertible_to<std::size_t>;
                { t.empty() } -> std::convertible_to<bool>;
        };

    // ----- Indexable (operator[])
    template <typename T, typename Index = std::size_t>
    concept Indexable =
        requires(T t, Index i) {
                { t[i] } -> std::convertible_to<decltype(t[i])>;
        };

    // ----- Key-value (map-like)
    template <typename T, typename Key, typename Value>
    concept KeyValueContainer =
        Container<T> &&
        requires(T t, Key k) {
                { t[k] } -> std::convertible_to<Value&>;
                { t.find(k) } -> std::convertible_to<decltype(t.find(k))>;
                { t.end() } -> std::sentinel_for<decltype(t.begin())>;
        };

    // ----- Numeric container (can sum all elements)
    template <typename T>
    concept NumericContainer =
        Container<T> &&
        Number<std::ranges::range_value_t<T>>;

    // ----- Sortable (can be sorted)
    template <typename T>
    concept Sortable =
        Container<T> &&
        Comparable<std::ranges::range_value_t<T>> &&
        requires(T t) {
                { std::sort(std::begin(t), std::end(t)) } -> std::convertible_to<void>;
        };

    // ----- Complex numbers
    template <typename T>
    concept Complex =
        requires(T t) {
                { t.real() } -> std::convertible_to<typename T::value_type>;
                { t.imag() } -> std::convertible_to<typename T::value_type>;
        };

    // ----- Duration (chrono)
    template <typename T>
    concept Duration =
        requires(T t) {
                { t.count() } -> std::convertible_to<typename T::rep>;
        };

    // ----- Math type (number, complex, or duration)
    template <typename T>
    concept MathType =
        Number<T> || Complex<T> || Duration<T>;

    // ----- Safe arithmetic type (no overflow/underflow)
    template <typename T>
    concept SafeType =
        SafeArithmetic<T> &&
        Number<T>;

    } // namespace hp