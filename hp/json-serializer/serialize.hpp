#pragma once

#include "json/parser.hpp"

#include <array>
#include <filesystem>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <meta>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace hp::ser {

    template <typename T>
    std::string to_json(const std::unique_ptr<T> &ptr);

    template <typename T>
    std::string to_json(const std::shared_ptr<T> &ptr);

    template <typename T>
    std::string to_json(const std::weak_ptr<T> &ptr);

    template <typename T, typename U>
    std::string to_json(const std::map<T, U> &map);

    template <typename T, typename U>
    std::string to_json(const std::pair<T, U> &pair);

    template <typename T>
    std::string to_json(const std::vector<T> &vec);

    template <typename T>
    std::string to_json(const std::set<T> &vec);

    template <typename T, std::size_t U>
    std::string to_json(const std::array<T, U> &arr);

    template <typename T>
    std::string to_json(const std::optional<T> &value);

    template <typename T>
        requires std::is_enum_v<T>
    std::string to_json(T value);

    template <typename T>
        requires(!std::is_enum_v<T>)
    std::string to_json(const T &value);

    template <typename T>
    T from_json_value(const JsonValue &v);

    template <typename T>
    T from_json(const std::string &text);

    inline std::string to_json(const int &value);
    inline std::string to_json(const double &value);
    inline std::string to_json(const bool &value);
    inline std::string to_json(const char &value);
    inline std::string to_json(const std::string &value);
    inline std::string to_json(const std::string_view &value);
    inline std::string to_json(const std::filesystem::path &path);

    template <typename>
    struct is_vector : std::false_type {};
    template <typename U>
    struct is_vector<std::vector<U>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_vector_v = is_vector<T>::value;

    template <typename>
    struct is_map : std::false_type {};
    template <typename T, typename U>
    struct is_map<std::map<T, U>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_map_v = is_map<T>::value;

    template <typename>
    struct is_unique_ptr : std::false_type {};
    template <typename T>
    struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

    template <typename>
    struct is_shared_ptr : std::false_type {};
    template <typename T>
    struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

    template <typename>
    struct is_weak_ptr : std::false_type {};
    template <typename T>
    struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_weak_ptr_v = is_weak_ptr<T>::value;

    template <typename>
    struct is_optional : std::false_type {};
    template <typename U>
    struct is_optional<std::optional<U>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_optional_v = is_optional<T>::value;

    template <typename>
    struct is_pair : std::false_type {};
    template <typename T, typename U>
    struct is_pair<std::pair<T, U>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_pair_v = is_pair<T>::value;

    inline std::string to_json(const int &value) {
        return std::format("{}", value);
    }

    inline std::string to_json(const double &value) {
        return std::format("{}", value);
    }

    inline std::string to_json(const bool &value) {
        return value ? "true" : "false";
    }

    inline std::string to_json(const std::filesystem::path &path) {
        return to_json(path.generic_string());
    }

    template <typename T>
    inline std::string to_json(const std::optional<T> &value) {
        if (!value.has_value()) {
            return "null";
        }
        return to_json(*value);
    }

    inline std::string to_json(const std::string &value) {
        std::string result = "\"";
        for (char c : value) {
            switch (c) {
                case '"':
                    result += "\\\"";
                    break;
                case '\\':
                    result += "\\\\";
                    break;
                case '\n':
                    result += "\\n";
                    break;
                case '\t':
                    result += "\\t";
                    break;
                case '\r':
                    result += "\\r";
                    break;
                default:
                    result += c;
            }
        }
        result += "\"";
        return result;
    }

    inline std::string to_json(const std::string_view &value) {
        return to_json(std::string(value));
    }

    inline std::string to_json(const char &value) {
        return to_json(std::string(1, value));
    }

    template <typename T>
    std::string to_json(const std::vector<T> &vec) {
        std::string result = "[";
        bool first = true;
        for (const auto &v : vec) {
            if (!first)
                result += ", ";
            first = false;
            result += to_json(v);
        }
        result += "]";
        return result;
    }

    template <typename T>
    std::string to_json(const std::set<T> &vec) {
        std::string result = "[";
        bool first = true;
        for (const auto &v : vec) {
            if (!first)
                result += ", ";
            first = false;
            result += to_json(v);
        }
        result += "]";
        return result;
    }

    template <typename T, std::size_t U>
    std::string to_json(const std::array<T, U> &arr) {
        std::string result = "[";
        bool first = true;
        for (const auto &a : arr) {
            if (!first) {
                result += ", ";
            }
            first = false;
            result += to_json(a);
        }
        return result += "]";
    }

    template <typename T>
    std::string to_json(const std::unique_ptr<T> &ptr) {
        if (!ptr)
            return "null";
        return to_json(*ptr);
    }

    template <typename T>
    std::string to_json(const std::shared_ptr<T> &ptr) {
        if (!ptr)
            return "null";
        return to_json(*ptr);
    }

    template <typename T>
    std::string to_json(const std::weak_ptr<T> &p) {
        if (auto sp = p.lock()) {
            return to_json(*sp);
        }
        return "null";
    }

    template <typename T, typename U>
    std::string to_json(const std::map<T, U> &map) {
        std::string result;
        bool first = true;
        result += "{";
        for (const auto &[Key, Value] : map) {
            if (!first)
                result += ", ";
            first = false;
            result += to_json(Key);
            result += ": ";
            result += to_json(Value);
        }

        result += "}";
        return result;
    }

    template <typename T, typename U>
    std::string to_json(const std::pair<T, U> &pair) {
        std::string result = "[";

        result += to_json(pair.first);
        result += ", ";
        result += to_json(pair.second);

        return result += "]";
    }

    template <typename T>
        requires std::is_enum_v<T>
    std::string to_json(T value) {
        static constexpr auto enumerators = std::define_static_array(
            std::meta::enumerators_of(^^T));

        template for (constexpr auto e : enumerators) {
            if (value == [:e:]) {
                return std::format("\"{}\"", std::meta::identifier_of(e));
            }
        }
        return "null";
    }

    template <typename T>
        requires(!std::is_enum_v<T>)
    std::string to_json(const T &value) {
        static constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));

        std::string result = "{";
        bool first = true;

        template for (constexpr auto m : members) {
            if (!first)
                result += ", ";
            first = false;
            result += std::format("\"{}\": ", std::meta::identifier_of(m));
            result += to_json(value.[:m:]);
        }

        result += "}";
        return result;
    }

    template <typename T>
    T from_json_value(const JsonValue &v) {
        using DecayedT = std::decay_t<T>;

        if constexpr (std::is_same_v<DecayedT, int>) {
            if (!std::holds_alternative<double>(v.data))
                throw std::runtime_error("Expected number for int");
            return static_cast<int>(std::get<double>(v.data));

        } else if constexpr (std::is_same_v<DecayedT, double>) {
            if (!std::holds_alternative<double>(v.data))
                throw std::runtime_error("Expected number");
            return std::get<double>(v.data);

        } else if constexpr (std::is_same_v<DecayedT, std::string>) {
            if (!std::holds_alternative<std::string>(v.data))
                throw std::runtime_error("Expected string");
            return std::get<std::string>(v.data);

        } else if constexpr (std::is_same_v<DecayedT, bool>) {
            if (!std::holds_alternative<bool>(v.data))
                throw std::runtime_error("Expected bool");
            return std::get<bool>(v.data);

        } else if constexpr (is_vector_v<DecayedT>) {
            using Elem = typename T::value_type;
            if (!std::holds_alternative<std::vector<JsonValuePtr>>(v.data))
                throw std::runtime_error("Expected array");
            const auto &arr = std::get<std::vector<JsonValuePtr>>(v.data);

            T result;
            result.reserve(arr.size());
            for (const auto &elem : arr) {
                result.push_back(from_json_value<Elem>(*elem));
            }
            return result;

        } else if constexpr (is_map_v<DecayedT>) {
            using V = typename T::mapped_type;
            if (!std::holds_alternative<std::map<std::string, JsonValuePtr>>(v.data))
                throw std::runtime_error("Expected object (map)");
            const auto &obj = std::get<std::map<std::string, JsonValuePtr>>(v.data);

            T result;
            for (const auto &[key, val] : obj) {
                result[key] = from_json_value<V>(*val);
            }
            return result;

        } else if constexpr (is_unique_ptr_v<DecayedT>) {
            using Elem = typename T::element_type;
            if (std::holds_alternative<std::nullptr_t>(v.data))
                return nullptr;
            return std::make_unique<Elem>(from_json_value<Elem>(v));

        } else if constexpr (is_shared_ptr_v<DecayedT>) {
            using Elem = typename T::element_type;
            if (std::holds_alternative<std::nullptr_t>(v.data))
                return nullptr;

            return std::make_shared<Elem>(from_json_value<Elem>(v));

        } else if constexpr (is_weak_ptr_v<DecayedT>) {
            using Elem = typename T::element_type;
            if (std::holds_alternative<std::nullptr_t>(v.data))
                return DecayedT();

            const auto &sp = std::make_shared<Elem>(from_json_value<Elem>(v));
            static_assert(sizeof(T) == 0, "Hi, if u see this it means ur using weak_ptr, pls go touch grass outside, think twice abt ur code design and dont use this shit anymore,thanks.\n\n");

        } else if constexpr (is_optional_v<DecayedT>) {
            using Elem = typename T::value_type;
            if (std::holds_alternative<std::nullptr_t>(v.data))
                return std::nullopt;
            return from_json_value<Elem>(v);

        } else if constexpr (is_pair_v<DecayedT>) {
            using First = typename T::first_type;
            using Second = typename T::second_type;

            if (!std::holds_alternative<std::vector<JsonValuePtr>>(v.data))
                throw std::runtime_error("expected array for pair");

            const auto &arr = std::get<std::vector<JsonValuePtr>>(v.data);
            if (arr.size() != 2)
                throw std::runtime_error("expected 2 elements for pair");

            return T{
                from_json_value<First>(*arr[0]),
                from_json_value<Second>(*arr[1])};
        } else if constexpr (std::is_enum_v<DecayedT>) {
            static constexpr auto enumerators = std::define_static_array(
                std::meta::enumerators_of(^^T));

            template for (constexpr auto e : enumerators) {
                return [:e:];
            }
            return DecayedT();

        } else {
            if (!std::holds_alternative<std::map<std::string, JsonValuePtr>>(v.data))
                throw std::runtime_error("Expected object");

            const auto &obj = std::get<std::map<std::string, JsonValuePtr>>(v.data);
            T result{};

            static constexpr auto members = std::define_static_array(
                std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));

            template for (constexpr auto m : members) {
                std::string key(std::meta::identifier_of(m));
                const auto &it = obj.find(key);
                if (it == obj.end())
                    continue;
                using FieldType = std::remove_cvref_t<decltype(result.[:m:])>;
                result.[:m:] = from_json_value<FieldType>(*it->second);
            }

            return result;
        }
    }

    template <typename T>
    T from_json(const std::string &text) {
        Parser p(text);
        auto tree = p.parse();
        return from_json_value<T>(*tree);
    }

} // namespace hp::ser