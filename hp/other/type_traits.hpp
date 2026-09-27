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

} // namespace hp