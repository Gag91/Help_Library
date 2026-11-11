#pragma once

#include "hp/concepts/concept.hpp"
#include "hp/other/type_traits.hpp"
#include <filesystem>
#include <functional>
#include <iostream>
#include <meta>
#include <type_traits>

#ifdef _WIN32
#include <windows.h>
#endif

namespace hp::detail {

    // ----- For UTF8 Encoding
    inline std::string char32_to_utf8(char32_t cp) {
        std::string out;
        if (cp <= 0x7F) {
            out.push_back(static_cast<char>(cp));
        } else if (cp <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else if (cp <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else if (cp <= 0x10FFFF) {
            out.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        return out;
    }
} // namespace hp::detail

namespace hp {

    namespace str {

        // ----- String to upper
        [[nodiscard]] inline std::string to_upper(std::string_view str) {
            std::string result;
            result.reserve(str.size());
            for (char c : str) {
                result.push_back(static_cast<unsigned char>(std::toupper(static_cast<unsigned char>(c))));
            }
            return result;
        }

        // ----- String to lower
        [[nodiscard]] inline std::string to_lower(std::string_view str) {
            std::string result;
            result.reserve(str.size());
            for (char c : str) {
                result.push_back(static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(c))));
            }
            return result;
        }

        // ----- String to T
        template <typename T>
        T from_string(const std::string &str) {
            using DecayedT = std::remove_cvref_t<T>;

            if constexpr (std::is_same_v<DecayedT, std::string>) {
                std::string result;
                for (const char &c : str) {
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
                            break;
                    }
                }
                return result;

            } else if constexpr (std::is_same_v<DecayedT, std::string_view>) {
                return std::string_view(str);

            } else if constexpr (std::is_same_v<DecayedT, std::filesystem::path>) {
                return std::filesystem::path(str);

            } else if constexpr (std::is_same_v<DecayedT, char>) {
                if (str.empty()) {
                    throw std::invalid_argument("Empty string cannot be converted to char");
                }
                return str[0];

            } else if constexpr (std::is_same_v<DecayedT, bool>) {
                if (to_lower(str) == "true" || str == "1")
                    return true;
                if (to_lower(str) == "false" || str == "0")
                    return false;
                throw std::invalid_argument("Invalid boolean string: " + str);

            } else if constexpr (std::is_same_v<DecayedT, int>) {
                return std::stoi(str);

            } else if constexpr (std::is_same_v<DecayedT, long>) {
                return std::stol(str);

            } else if constexpr (std::is_same_v<DecayedT, long long>) {
                return std::stoll(str);

            } else if constexpr (std::is_same_v<DecayedT, unsigned long>) {
                return std::stoul(str);

            } else if constexpr (std::is_same_v<DecayedT, unsigned long long>) {
                return std::stoull(str);

            } else if constexpr (std::is_same_v<DecayedT, float>) {
                return std::stof(str);

            } else if constexpr (std::is_same_v<DecayedT, double>) {
                return std::stod(str);

            } else if constexpr (std::is_same_v<DecayedT, long double>) {
                return std::stold(str);

            } else if constexpr (std::is_same_v<DecayedT, short>) {
                return static_cast<short>(std::stoi(str));

            } else if constexpr (std::is_same_v<DecayedT, unsigned short>) {
                return static_cast<unsigned short>(std::stoul(str));

            } else if constexpr (std::is_same_v<DecayedT, int8_t> ||
                                 std::is_same_v<DecayedT, signed char>) {
                return static_cast<int8_t>(std::stoi(str));

            } else if constexpr (std::is_same_v<DecayedT, uint8_t> ||
                                 std::is_same_v<DecayedT, unsigned char>) {
                return static_cast<uint8_t>(std::stoul(str));

            } else if constexpr (std::is_enum_v<DecayedT>) {
                static constexpr auto members = std::define_static_array(
                    std::meta::enumerators_of(^^DecayedT));

                template for (constexpr auto m : members) {
                    if (str == std::meta::identifier_of(m)) {
                        return [:m:];
                    }
                }
                throw std::invalid_argument("Invalid enum string: " + str);

            } else if constexpr (hp::Container<DecayedT>) {
                using ValueT = typename DecayedT::value_type;
                DecayedT result{};

                std::string trimmed = str;
                if (trimmed.size() >= 2 && trimmed.front() == '[' && trimmed.back() == ']') {
                    trimmed = trimmed.substr(1, trimmed.size() - 2);
                }

                std::size_t start = 0;
                std::size_t idx = 0;
                int depth = 0;

                for (std::size_t i = 0; i <= trimmed.size(); ++i) {
                    if (i < trimmed.size()) {
                        if (trimmed[i] == '[' || trimmed[i] == '(')
                            ++depth;
                        else if (trimmed[i] == ']' || trimmed[i] == ')')
                            --depth;
                    }

                    if (i == trimmed.size() || (trimmed[i] == ',' && depth == 0)) {
                        std::string token = trimmed.substr(start, i - start);
                        std::size_t first = token.find_first_not_of(" \t");
                        if (first != std::string::npos) {
                            std::size_t last = token.find_last_not_of(" \t");
                            token = token.substr(first, last - first + 1);

                            ValueT v = hp::str::from_string<ValueT>(token);

                            if constexpr (requires { result.push_back(v); }) {
                                result.push_back(v);
                            } else if constexpr (requires { result.insert(v); }) {
                                result.insert(v);
                            } else if constexpr (requires { result[idx] = v; }) {
                                result[idx] = v;
                            }
                            ++idx;
                        }
                        start = i + 1;
                    }
                }
                return result;

            } else {
                throw std::runtime_error("Unsupported type");
            }
        }

        // ----- T to String
        template <typename T>
        std::string to_string(const T &value) {
            using DecayedT = std::remove_cvref_t<T>;

            if constexpr (std::is_same_v<DecayedT, std::string>) {
                return value;

            } else if constexpr (std::is_same_v<DecayedT, std::string_view>) {
                return std::string(value);

            } else if constexpr (std::is_same_v<DecayedT, const char *> || std::is_same_v<DecayedT, char *>) {
                return value ? std::string(value) : "nullptr";

            } else if constexpr (std::is_same_v<DecayedT, std::filesystem::path>) {
                return value.string();

            } else if constexpr (std::is_same_v<DecayedT, char>) {
                return std::string(1, value);

            } else if constexpr (std::is_same_v<DecayedT, const char *> || std::is_same_v<DecayedT, char *>) {
                return value ? std::string(value) : "nullptr";
            } else if constexpr (std::is_array_v<DecayedT> && std::is_same_v<std::remove_cv_t<std::remove_extent_t<DecayedT>>, char>) {
                return std::string(value);
            } else if constexpr (std::is_same_v<DecayedT, char32_t>) {
                return hp::detail::char32_to_utf8(value);
            } else if constexpr (std::is_same_v<DecayedT, bool>) {
                return value ? "true" : "false";
            } else if constexpr (hp::any_of<DecayedT, short, int, long, long long, unsigned short,
                                            unsigned int, unsigned long, unsigned long long,
                                            float, double, long double>()) {
                return std::to_string(value);
            } else if constexpr (std::is_enum_v<DecayedT>) {
                static constexpr auto enums = std::define_static_array(std::meta::enumerators_of(^^DecayedT));
                template for (constexpr auto &e : enums) {
                    if (value == [:e:]) {
                        return std::string(std::meta::identifier_of(e));
                    }
                }
                return std::to_string(static_cast<std::underlying_type_t<DecayedT>>(value));
            } else if constexpr (hp::is_weak_ptr_v<DecayedT>) {
                auto locked = value.lock();
                if (locked == nullptr)
                    return "nullptr";
                return hp::str::to_string(*locked);
            } else if constexpr (hp::is_unique_ptr_v<DecayedT> ||
                                 hp::is_shared_ptr_v<DecayedT>) {
                if (value == nullptr)
                    return "nullptr";
                return hp::str::to_string(*value);
            } else if constexpr (requires { value.first; value.second; }) {
                return "(" + hp::str::to_string(value.first) + ": " + hp::str::to_string(value.second) + ")";
            } else if constexpr (hp::Container<DecayedT>) {
                std::string out = "[";
                bool first = true;
                for (const auto &item : value) {
                    if (!first) {
                        out += ", ";
                    }
                    out += hp::str::to_string(item);
                    first = false;
                }
                out += "]";
                return out;
            } else if constexpr (requires { value.to_string(); }) {
                return value.to_string();
            } else if constexpr (requires { std::to_string(value); }) {
                return std::to_string(value);
            } else if constexpr (std::is_class_v<DecayedT>) {
                static constexpr auto members = std::define_static_array(
                    std::meta::nonstatic_data_members_of(^^DecayedT, std::meta::access_context::unchecked()));

                if constexpr (members.size() == 0) {
                    return "{}";
                } else {
                    std::string result = "{";
                    bool first = true;
                    template for (constexpr auto m : members) {
                        if (!first)
                            result += ", ";
                        first = false;
                        result += hp::str::to_string(value.[:m:]);
                    }
                    result += "}";
                    return result;
                }
            } else {
                throw std::runtime_error("Unsupported type");
            }
        }

        // ----- Similar String
        template <typename T, typename U>
        inline bool is_typo(T value1, U value2) {
            std::string str1, str2;
            if constexpr (!std::is_same_v<std::decay_t<T>, std::string>) {
                str1 = hp::str::to_string(value1);
                str2 = hp::str::to_string(value2);
            } else {
                str1 = value1;
                str2 = value2;
            }

            if (str1 == str2)
                return false;

            std::string a = hp::str::to_lower(str1);
            std::string b = hp::str::to_lower(str2);

            if (a == b)
                return true;

            int errors = 0;
            if (a.size() == b.size()) {

                for (std::size_t i = 0; i < a.size(); ++i)
                    if (a[i] != b[i] && ++errors >= 2)
                        return false;
                return true;
            }
            if (a.size() > b.size())
                std::swap(a, b);
            if (b.size() != a.size() + 1)
                return false;

            std::size_t i = 0, j = 0;
            bool skipped = false;
            while (i < a.size()) {
                if (a[i] != b[j]) {
                    if (skipped)
                        return false;
                    skipped = true;
                    ++j;
                } else {
                    ++i;
                    ++j;
                }
            }

            return true;
        }

        namespace trim {

            // ----- White spaces
            [[nodiscard]] inline std::string white_spaces(std::string_view str) {
                std::string result;
                result.reserve(str.size());
                for (char c : str) {
                    if (c != ' ') {
                        result += c;
                    }
                }
                return result;
            }

            // ----- Numbers
            [[nodiscard]] inline std::string numbers(std::string_view str) {
                std::string result;
                result.reserve(str.size());
                for (char c : str) {
                    if (std::isdigit(static_cast<unsigned char>(c))) {
                        result += c;
                    }
                }
                return result;
            }

            // ----- Letters
            [[nodiscard]] inline std::string letters(std::string_view str) {
                std::string result;
                result.reserve(str.size());
                for (char c : str) {
                    if (std::isalpha(static_cast<unsigned char>(c))) {
                        result += c;
                    }
                }
                return result;
            }
        } // namespace trim
    } // namespace str

    // ----- Regular title function
    inline void title(const std::string &t, int w = 40) {
        std::string sep = std::string(w, '=');
        std::cout << sep << std::endl;
        std::cout << t << std::endl;
        std::cout << sep << std::endl;
    }
    // ----- Works the same as "std::string(int, char)" but maded to use string instead
    inline std::string repeatString(const std::string &str, int count) {
        std::string result;
        for (int i = 0; i < count; i++) {
            result += str;
        }
        return result;
    }

    // ----- Centered Seperator
    inline void sp(const std::string &msg, char a = '=') {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
        int consoleWidth = 80;
#endif
        int msgLen = msg.length() + 2;
        std::size_t totalWidth = consoleWidth;
        int sideLen = (totalWidth - msgLen) / 2;

        std::string result;
        result.reserve(totalWidth);
        result.append(sideLen, a);
        result += " " + msg + " ";
        result.append(sideLen, a);
        if (result.length() < totalWidth) {
            result += a;
        }
        std::cout << "\033[2K\r";
        std::cout << result << '\n';
    }
    // ----- Wait for enter key to be pressed
    inline void waitForEnter(std::function<void()> func = nullptr) {
        std::cout << "Press Enter To Continue..." << std::endl;
        std::cout.flush();
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        if (func) {
            func();
        }
    }

    inline bool starts_with(const std::string &str, const std::string &prefix) {
        return str.starts_with(prefix);
    }

    inline bool ends_with(const std::string &str, const std::string &suffix) {
        return str.ends_with(suffix);
    }

} // namespace hp