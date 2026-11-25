#pragma once

#include <map>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

namespace hp {

    // ----- Is Smart Pointer
    template <typename T>
    struct is_smart_pointer : std::false_type {};
    template <typename T>
    struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};
    template <typename T>
    struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_smart_pointer_v = is_smart_pointer<T>::value;

    // ----- Is Unique Pointer
    template <typename>
    struct is_unique_ptr : std::false_type {};
    template <typename T>
    struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

    // ----- Is Shared Pointer
    template <typename>
    struct is_shared_ptr : std::false_type {};
    template <typename T>
    struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

    // ----- Is Weak Pointer
    template <typename>
    struct is_weak_ptr : std::false_type {};
    template <typename T>
    struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_weak_ptr_v = is_weak_ptr<T>::value;

    // ----- Is Vector
    template <typename>
    struct is_vector : std::false_type {};
    template <typename U>
    struct is_vector<std::vector<U>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_vector_v = is_vector<T>::value;

    // ----- Is Map
    template <typename>
    struct is_map : std::false_type {};
    template <typename T, typename U>
    struct is_map<std::map<T, U>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_map_v = is_map<T>::value;

    // ----- Is Optional
    template <typename>
    struct is_optional : std::false_type {};
    template <typename U>
    struct is_optional<std::optional<U>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_optional_v = is_optional<T>::value;

    // ----- Is Pair
    template <typename>
    struct is_pair : std::false_type {};
    template <typename T, typename U>
    struct is_pair<std::pair<T, U>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_pair_v = is_pair<T>::value;

    // ----- Is any of
    template <typename T, typename... Args>
    constexpr bool any_of() {
        return ((std::is_same_v<T, Args>) || ...);
    }

    // ----- Remove const
    template <typename T>
    struct remove_const {
        using type = T;
    };
    template <typename T>
    struct remove_const<const T> {
        using type = T;
    };

    // ----- Add const
    template <typename T>
    struct add_const {
        using type = const T;
    };

    // ----- Add past-const
    template <typename T>
    struct add_past_const {
        using type = T const;
    };

    // ----- Is const predicate
    template <typename T>
    struct is_const {
        static constexpr bool value = false;
    };
    template <typename T>
    struct is_const<const T> {
        static constexpr bool value = true;
    };

    // ----- Add const under reference
    template <typename T>
    struct add_const_under_ref {
        using type = const T;
    };
    template <typename T>
    struct add_const_under_ref<T &> {
        using type = const T &;
    };
    template <typename T>
    struct add_const_under_ref<T &&> {
        using type = const T &&;
    };

    // ----- Remove const under reference
    template <typename T>
    struct remove_const_under_ref {
        using type = typename remove_const<T>::type;
    };
    template <typename T>
    struct remove_const_under_ref<T &> {
        using type = typename remove_const<T>::type &;
    };
    template <typename T>
    struct remove_const_under_ref<T &&> {
        using type = typename remove_const<T>::type &&;
    };

    // ----- Add pointer const
    template <typename T>
    struct add_pointer_const {
        using type = const T;
    };
    template <typename T>
    struct add_pointer_const<T *> {
        using type = const T *;
    };
    template <typename T>
    struct add_pointer_const<T *const> {
        using type = const T *const;
    };
    template <typename T>
    struct add_pointer_const<T *volatile> {
        using type = const T *volatile;
    };
    template <typename T>
    struct add_pointer_const<T *const volatile> {
        using type = const T *const volatile;
    };

    // ----- Remove pointer const
    template <typename T>
    struct remove_pointer_const {
        using type = typename remove_const<T>::type;
    };
    template <typename T>
    struct remove_pointer_const<T *> {
        using type = typename remove_const<T>::type *;
    };
    template <typename T>
    struct remove_pointer_const<T *const> {
        using type = typename remove_const<T>::type *const;
    };
    template <typename T>
    struct remove_pointer_const<T *volatile> {
        using type = typename remove_const<T>::type *volatile;
    };
    template <typename T>
    struct remove_pointer_const<T *const volatile> {
        using type = typename remove_const<T>::type *const volatile;
    };

    // ----- Add deep const
    template <typename T>
    struct add_deep_const {
        using type = const T;
    };
    template <typename T>
    struct add_deep_const<T *> {
        using type = typename add_deep_const<T>::type *const;
    };
    template <typename T>
    struct add_deep_const<T *const> {
        using type = typename add_deep_const<T>::type *const;
    };
    template <typename T>
    struct add_deep_const<T *volatile> {
        using type = typename add_deep_const<T>::type *const volatile;
    };
    template <typename T>
    struct add_deep_const<T *const volatile> {
        using type = typename add_deep_const<T>::type *const volatile;
    };
    template <typename T>
    struct add_deep_const<T &> {
        using type = typename add_deep_const<T>::type &;
    };
    template <typename T>
    struct add_deep_const<T &&> {
        using type = typename add_deep_const<T>::type &&;
    };

    // ----- Remove deep const
    template <typename T>
    struct remove_deep_const {
        using type = typename remove_const<T>::type;
    };
    template <typename T>
    struct remove_deep_const<T *> {
        using type = typename remove_deep_const<T>::type *;
    };
    template <typename T>
    struct remove_deep_const<T *const> {
        using type = typename remove_deep_const<T>::type *;
    };
    template <typename T>
    struct remove_deep_const<T *volatile> {
        using type = typename remove_deep_const<T>::type *volatile;
    };
    template <typename T>
    struct remove_deep_const<T *const volatile> {
        using type = typename remove_deep_const<T>::type *volatile;
    };
    template <typename T>
    struct remove_deep_const<T &> {
        using type = typename remove_deep_const<T>::type &;
    };
    template <typename T>
    struct remove_deep_const<T &&> {
        using type = typename remove_deep_const<T>::type &&;
    };

    // ----- Copy constness from From onto To
    template <typename From, typename To>
    struct copy_const {
        using type = typename remove_const<To>::type;
    };
    template <typename From, typename To>
    struct copy_const<const From, To> {
        using type = typename add_const<To>::type;
    };

    // ----- Conditional const
    template <bool Cond, typename T>
    struct add_const_if {
        using type = T;
    };
    template <typename T>
    struct add_const_if<true, T> {
        using type = const T;
    };

    // ----- Remove volatile
    template <typename T>
    struct remove_volatile {
        using type = T;
    };
    template <typename T>
    struct remove_volatile<volatile T> {
        using type = T;
    };

    // ----- Add volatile
    template <typename T>
    struct add_volatile {
        using type = volatile T;
    };

    // ----- Remove const volatile
    template <typename T>
    struct remove_cv {
        using type = typename remove_volatile<typename remove_const<T>::type>::type;
    };

    // ----- Add const volatile
    template <typename T>
    struct add_cv {
        using type = const volatile T;
    };

    // ----- Remove reference
    template <typename T>
    struct remove_reference {
        using type = T;
    };
    template <typename T>
    struct remove_reference<T &> {
        using type = T;
    };
    template <typename T>
    struct remove_reference<T &&> {
        using type = T;
    };

    // ----- Add lvalue reference
    template <typename T, typename = void>
    struct add_lvalue_reference {
        using type = T;
    };
    template <typename T>
    struct add_lvalue_reference<T, std::void_t<T &>> {
        using type = T &;
    };

    // ----- Add rvalue reference
    template <typename T, typename = void>
    struct add_rvalue_reference {
        using type = T;
    };
    template <typename T>
    struct add_rvalue_reference<T, std::void_t<T &&>> {
        using type = T &&;
    };

    // ----- Remove cvref
    template <typename T>
    struct remove_cvref {
        using type = typename remove_cv<typename remove_reference<T>::type>::type;
    };

    // ----- Remove pointer
    template <typename T>
    struct remove_pointer {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T *> {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T *const> {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T *volatile> {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T *const volatile> {
        using type = T;
    };

    // ----- Add pointer
    template <typename T, typename = void>
    struct add_pointer {
        using type = T;
    };

    template <typename T>
    struct add_pointer<T, std::void_t<typename remove_reference<T>::type *>> {
        using type = typename remove_reference<T>::type *;
    };

    // ----- Remove extent
    template <typename T>
    struct remove_extent {
        using type = T;
    };
    template <typename T>
    struct remove_extent<T[]> {
        using type = T;
    };
    template <typename T, std::size_t N>
    struct remove_extent<T[N]> {
        using type = T;
    };

    // ----- Remove all extents
    template <typename T>
    struct remove_all_extents {
        using type = T;
    };
    template <typename T>
    struct remove_all_extents<T[]> {
        using type = typename remove_all_extents<T>::type;
    };
    template <typename T, std::size_t N>
    struct remove_all_extents<T[N]> {
        using type = typename remove_all_extents<T>::type;
    };

    // ----- Aliases
    template <typename T>
    inline constexpr bool is_const_v = is_const<T>::value;
    template <typename T>
    using remove_const_t = typename remove_const<T>::type;
    template <typename T>
    using add_const_t = typename add_const<T>::type;
    template <typename T>
    using add_past_const_t = typename add_past_const<T>::type;
    template <typename T>
    using add_const_under_ref_t = typename add_const_under_ref<T>::type;
    template <typename T>
    using remove_const_under_ref_t = typename remove_const_under_ref<T>::type;
    template <typename T>
    using add_pointer_const_t = typename add_pointer_const<T>::type;
    template <typename T>
    using remove_pointer_const_t = typename remove_pointer_const<T>::type;
    template <typename T>
    using add_deep_const_t = typename add_deep_const<T>::type;
    template <typename T>
    using remove_deep_const_t = typename remove_deep_const<T>::type;
    template <typename From, typename To>
    using copy_const_t = typename copy_const<From, To>::type;
    template <bool Cond, typename T>
    using add_const_if_t = typename add_const_if<Cond, T>::type;
    template <typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;
    template <typename T>
    using add_volatile_t = typename add_volatile<T>::type;
    template <typename T>
    using remove_cv_t = typename remove_cv<T>::type;
    template <typename T>
    using add_cv_t = typename add_cv<T>::type;
    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;
    template <typename T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
    template <typename T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;
    template <typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;
    template <typename T>
    using remove_pointer_t = typename remove_pointer<T>::type;
    template <typename T>
    using add_pointer_t = typename add_pointer<T>::type;
    template <typename T>
    using remove_extent_t = typename remove_extent<T>::type;
    template <typename T>
    using remove_all_extents_t = typename remove_all_extents<T>::type;

} // namespace hp