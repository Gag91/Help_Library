#pragma once

#include "hp/other/type_traits.hpp"
#include "json/parser.hpp"

#include <array>
#include <filesystem>
#include <format>
#include <fstream>
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

    namespace detail {

        template <typename T>
            requires(!std::is_enum_v<T>)
        std::string to_json_impl(const T &value, int level, int step);

        template <typename T>
        std::string to_json_impl(const std::unique_ptr<T> &ptr, int level, int step);

        template <typename T>
        std::string to_json_impl(const std::shared_ptr<T> &ptr, int level, int step);

        template <typename T>
        std::string to_json_impl(const std::weak_ptr<T> &ptr, int level, int step);

        template <typename T, typename U>
        std::string to_json_impl(const std::map<T, U> &map, int level, int step);

        template <typename T, typename U>
        std::string to_json_impl(const std::pair<T, U> &pair, int level, int step);

        template <typename T>
        std::string to_json_impl(const std::vector<T> &vec, int level, int step);

        template <typename T>
        std::string to_json_impl(const std::set<T> &vec, int level, int step);

        template <typename T, std::size_t U>
        std::string to_json_impl(const std::array<T, U> &arr, int level, int step);

        template <typename T>
        std::string to_json_impl(const std::optional<T> &value, int level, int step);

        inline std::string to_json_impl(const int &value, int level, int step);
        inline std::string to_json_impl(const double &value, int level, int step);
        inline std::string to_json_impl(const bool &value, int level, int step);
        inline std::string to_json_impl(const std::string &value, int level, int step);
        inline std::string to_json_impl(const char &value, int level, int step);
        inline std::string to_json_impl(const std::string_view &value, int level, int step);
        inline std::string to_json_impl(const std::filesystem::path &path, int level, int step);

        template <typename T>
            requires std::is_enum_v<T>
        std::string to_json_impl(const T &value, int level, int step);

        template <typename T>
            requires(!std::is_enum_v<T>)
        std::string to_json_impl(const T &value, int level, int step);

        inline std::string indent_of(int level, int step) {
            if (step <= 0 || level <= 0)
                return "";
            return std::string(static_cast<std::size_t>(level) * static_cast<std::size_t>(step), ' ');
        }

        inline std::string to_json_impl(const int &value, int, int) {
            return std::format("{}", value);
        }

        inline std::string to_json_impl(const double &value, int, int) {
            return std::format("{}", value);
        }

        inline std::string to_json_impl(const bool &value, int, int) {
            return value ? "true" : "false";
        }

        inline std::string to_json_impl(const std::filesystem::path &path, int, int) {
            return to_json_impl(path.generic_string(), 0, 0);
        }

        inline std::string to_json_impl(const std::string &value, int, int) {
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

        inline std::string to_json_impl(const std::string_view &value, int, int) {
            return to_json_impl(std::string(value), 0, 0);
        }

        inline std::string to_json_impl(const char &value, int, int) {
            return to_json_impl(std::string(1, value), 0, 0);
        }

        template <typename T>
        std::string to_json_impl(const std::vector<T> &vec, int level, int step) {
            if (vec.empty())
                return "[]";

            if (step <= 0) {
                std::string result = "[";
                bool first = true;
                for (const auto &v : vec) {
                    if (!first)
                        result += ", ";
                    first = false;
                    result += to_json_impl(v, 0, 0);
                }
                result += "]";
                return result;
            }

            std::string inner = indent_of(level + 1, step);
            std::string closing = indent_of(level, step);
            std::string result = "[\n";
            bool first = true;
            for (const auto &v : vec) {
                if (!first)
                    result += ",\n";
                first = false;
                result += inner + to_json_impl(v, level + 1, step);
            }
            result += "\n" + closing + "]";
            return result;
        }

        template <typename T>
        std::string to_json_impl(const std::set<T> &vec, int level, int step) {
            if (vec.empty())
                return "[]";

            if (step <= 0) {
                std::string result = "[";
                bool first = true;
                for (const auto &v : vec) {
                    if (!first)
                        result += ", ";
                    first = false;
                    result += to_json_impl(v, 0, 0);
                }
                result += "]";
                return result;
            }

            std::string inner = indent_of(level + 1, step);
            std::string closing = indent_of(level, step);
            std::string result = "[\n";
            bool first = true;
            for (const auto &v : vec) {
                if (!first)
                    result += ",\n";
                first = false;
                result += inner + to_json_impl(v, level + 1, step);
            }
            result += "\n" + closing + "]";
            return result;
        }

        template <typename T, std::size_t U>
        std::string to_json_impl(const std::array<T, U> &arr, int level, int step) {
            if constexpr (U == 0) {
                return "[]";
            } else {
                if (step <= 0) {
                    std::string result = "[";
                    bool first = true;
                    for (const auto &a : arr) {
                        if (!first)
                            result += ", ";
                        first = false;
                        result += to_json_impl(a, 0, 0);
                    }
                    result += "]";
                    return result;
                }

                std::string inner = indent_of(level + 1, step);
                std::string closing = indent_of(level, step);
                std::string result = "[\n";
                bool first = true;
                for (const auto &a : arr) {
                    if (!first)
                        result += ",\n";
                    first = false;
                    result += inner + to_json_impl(a, level + 1, step);
                }
                result += "\n" + closing + "]";
                return result;
            }
        }

        template <typename T, typename U>
        std::string to_json_impl(const std::map<T, U> &map, int level, int step) {
            if (map.empty())
                return "{}";

            if (step <= 0) {
                std::string result = "{";
                bool first = true;
                for (const auto &[k, v] : map) {
                    if (!first)
                        result += ", ";
                    first = false;
                    result += to_json_impl(k, 0, 0) + ": " + to_json_impl(v, 0, 0);
                }
                result += "}";
                return result;
            }

            std::string inner = indent_of(level + 1, step);
            std::string closing = indent_of(level, step);
            std::string result = "{\n";
            bool first = true;
            for (const auto &[k, v] : map) {
                if (!first)
                    result += ",\n";
                first = false;
                result += inner + to_json_impl(k, level + 1, step) + ": " + to_json_impl(v, level + 1, step);
            }
            result += "\n" + closing + "}";
            return result;
        }

        template <typename T, typename U>
        std::string to_json_impl(const std::pair<T, U> &pair, int level, int step) {
            if (step <= 0) {
                return "[" + to_json_impl(pair.first, 0, 0) + ", " + to_json_impl(pair.second, 0, 0) + "]";
            }

            std::string inner = indent_of(level + 1, step);
            std::string closing = indent_of(level, step);
            return "[\n" + inner + to_json_impl(pair.first, level + 1, step) +
                   ",\n" + inner + to_json_impl(pair.second, level + 1, step) +
                   "\n" + closing + "]";
        }

        template <typename T>
        std::string to_json_impl(const std::unique_ptr<T> &ptr, int level, int step) {
            if (!ptr)
                return "null";
            return to_json_impl(*ptr, level, step);
        }

        template <typename T>
        std::string to_json_impl(const std::shared_ptr<T> &ptr, int level, int step) {
            if (!ptr)
                return "null";
            return to_json_impl(*ptr, level, step);
        }

        template <typename T>
        std::string to_json_impl(const std::weak_ptr<T> &ptr, int level, int step) {
            auto sp = ptr.lock();
            if (!sp)
                return "null";
            return to_json_impl(*sp, level, step);
        }

        template <typename T>
        std::string to_json_impl(const std::optional<T> &value, int level, int step) {
            if (!value.has_value())
                return "null";
            return to_json_impl(*value, level, step);
        }

        template <typename T>
            requires std::is_enum_v<T>
        std::string to_json_impl(const T &value, int, int) {
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
        std::string to_json_impl(const T &value, int level, int step) {
            static constexpr auto members = std::define_static_array(
                std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));

            if (step <= 0) {
                std::string result = "{";
                bool first = true;
                template for (constexpr auto m : members) {
                    if (!first)
                        result += ", ";
                    first = false;
                    result += std::format("\"{}\": ", std::meta::identifier_of(m));
                    result += to_json_impl(value.[:m:], 0, 0);
                }
                result += "}";
                return result;
            }

            std::string inner = indent_of(level + 1, step);
            std::string closing = indent_of(level, step);
            std::string result = "{\n";
            bool first = true;
            template for (constexpr auto m : members) {
                if (!first)
                    result += ",\n";
                first = false;
                result += inner + std::format("\"{}\": ", std::meta::identifier_of(m));
                result += to_json_impl(value.[:m:], level + 1, step);
            }
            result += "\n" + closing + "}";
            return result;
        }

    } // namespace detail

    template <typename T>
    std::string to_json(const T &value) {
        return detail::to_json_impl(value, 0, 0);
    }

    template <typename T>
    std::string to_json(const T &value, int pretty_step) {
        return detail::to_json_impl(value, 0, pretty_step);
    }

    template <typename T>
    std::string to_json_pretty(const T &value, int step = 2) {
        return detail::to_json_impl(value, 0, step);
    }

    template <typename T>
    bool to_json_file(const std::filesystem::path &path, const T &value, bool pretty = true) {
        std::ofstream out(path, std::ios::binary);
        if (!out)
            return false;
        out << to_json(value, pretty ? 2 : 0);
        return static_cast<bool>(out);
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
            static_assert(sizeof(T) == 0, "weak_ptr is not supported by from_json; store a shared_ptr instead");

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