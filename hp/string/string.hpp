#pragma once

#include "hp/concepts/concept.hpp"
#include "hp/other/integers.hpp"
#include "hp/other/type_traits.hpp"

#include <algorithm>
#include <cctype>
#include <concepts>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <meta>
#include <ranges>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
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

        // ----- Trim whitespace from both ends, returning a view
        [[nodiscard]] inline std::string_view trim_view(std::string_view s) {
            while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front())))
                s.remove_prefix(1);
            while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
                s.remove_suffix(1);
            return s;
        }

        // ----- Trim whitespace from both ends, returning a copy
        [[nodiscard]] inline std::string str_trim(std::string_view s) {
            return std::string(trim_view(s));
        }

        // ----- String to upper
        [[nodiscard]] inline std::string to_upper(std::string_view str) {
            return str | std::views::transform([](unsigned char c) {
                       return static_cast<char>(std::toupper(c));
                   }) |
                   std::ranges::to<std::string>();
        }

        // ----- String to lower
        [[nodiscard]] inline std::string to_lower(std::string_view str) {
            return str | std::views::transform([](unsigned char c) {
                       return static_cast<char>(std::tolower(c));
                   }) |
                   std::ranges::to<std::string>();
        }

        // ----- String to T
        template <typename T>
        T from_string(const std::string &str) {
            using DecayedT = std::remove_cvref_t<T>;

            if constexpr (std::is_same_v<DecayedT, std::string>) {
                return str;

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
                const std::string lower = to_lower(str);
                if (lower == "true" || str == "1")
                    return true;
                if (lower == "false" || str == "0")
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

        // ----- constexpr to string for integers
        template <std::integral T>
        [[nodiscard]] constexpr std::string int_to_string(T val) {
            constexpr std::size_t buf_size = std::numeric_limits<T>::digits10 + 3;
            char buf[buf_size];
            const auto [ptr, ec] = std::to_chars(buf, buf + buf_size, val);
            return std::string(buf, ptr);
        }

        // ----- constexpr to string for floating points
        template <std::floating_point T>
        [[nodiscard]] constexpr std::string float_to_string(T val) {
            if consteval {
                if (val == 0) {
                    return "0";
                }

                std::string res;
                if (val < 0) {
                    res += '-';
                    val = -val;
                }
                auto int_part = static_cast<unsigned long long>(val);
                T frac = val - static_cast<T>(int_part);

                if (int_part == 0) {
                    res += '0';
                } else {
                    std::string digits;
                    while (int_part > 0) {
                        digits += static_cast<char>('0' + (int_part % 10));
                        int_part /= 10;
                    }
                    for (std::size_t i = digits.size(); i > 0; --i) {
                        res += digits[i - 1];
                    }
                }

                if (frac > 0) {
                    res += '.';
                    for (int i = 0; i < 6 && frac > static_cast<T>(1e-7); ++i) {
                        frac *= 10;
                        int digit = static_cast<int>(frac);
                        res += static_cast<char>('0' + digit);
                        frac -= digit;
                    }
                }
                return res;
            } else {
                std::array<char, 64> buf{};
                const auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), val);
                return std::string(buf.data(), ptr);
            }
        }

        // ----- T to String
        template <typename T>
        [[nodiscard]] constexpr std::string to_string(const T &value) {
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
            } else if constexpr (std::is_integral_v<DecayedT>) {
                return int_to_string(value);
            } else if constexpr (std::is_floating_point_v<DecayedT>) {
                return float_to_string(value);
            } else if constexpr (std::is_enum_v<DecayedT>) {
                static constexpr auto enums = std::define_static_array(std::meta::enumerators_of(^^DecayedT));
                template for (constexpr auto &e : enums) {
                    if (value == [:e:]) {
                        return std::string(std::meta::identifier_of(e));
                    }
                }
                return int_to_string(static_cast<std::underlying_type_t<DecayedT>>(value));
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
                    if (!first)
                        out += ", ";
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
            } else if constexpr (requires { std::format("{}", value); }) {
                return std::format("{}", value);
            } else {
                static_assert(false, "hp::str::to_string: unsupported type");
            }
        }

        // ----- Similar String
        template <typename T, typename U>
        inline bool is_typo(T value1, U value2) {
            std::string str1 = hp::str::to_string(value1);
            std::string str2 = hp::str::to_string(value2);

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

        // ----- Remove surrounding "…" or '…' and unescape \" \\ \n \t \r
        [[nodiscard]] inline std::string unquote(std::string_view s) {
            s = trim_view(s);
            if (s.size() < 2)
                return std::string(s);
            char q = s.front();
            if ((q != '"' && q != '\'') || s.back() != q)
                return std::string(s);

            std::string out;
            out.reserve(s.size() - 2);
            for (std::size_t i = 1; i + 1 < s.size(); ++i) {
                if (s[i] == '\\' && i + 2 < s.size()) {
                    switch (s[++i]) {
                        case 'n':
                            out += '\n';
                            break;
                        case 't':
                            out += '\t';
                            break;
                        case 'r':
                            out += '\r';
                            break;
                        case '\\':
                            out += '\\';
                            break;
                        case '"':
                            out += '"';
                            break;
                        case '\'':
                            out += '\'';
                            break;
                        default:
                            out += s[i];
                            break;
                    }
                } else {
                    out += s[i];
                }
            }
            return out;
        }

        namespace trim {

            // ----- White spaces
            [[nodiscard]] inline std::string white_spaces(std::string_view str) {
                return str | std::views::filter([](char c) { return c != ' '; }) | std::ranges::to<std::string>();
            }

            // ----- Numbers
            [[nodiscard]] inline std::string numbers(std::string_view str) {
                return str | std::views::filter([](char c) {
                           return std::isdigit(static_cast<unsigned char>(c)) != 0;
                       }) |
                       std::ranges::to<std::string>();
            }

            // ----- Letters
            [[nodiscard]] inline std::string letters(std::string_view str) {
                return str | std::views::filter([](char c) {
                           return std::isalpha(static_cast<unsigned char>(c)) != 0;
                       }) |
                       std::ranges::to<std::string>();
            }

            // ----- ANSI regex pattern
            inline const std::regex &ansi_regex() {
                static const std::regex re(
                    R"(\x1B(?:\[[0-9;?]*[ -/]*[@-~]|\][^\x07]*(?:\x07|\x1B\\)|[()][0-2A-Z]|[=>NOPc]))",
                    std::regex::ECMAScript | std::regex::optimize);
                return re;
            }

            // ----- Strip ANSI
            [[nodiscard]] inline std::string ansi(std::string_view input) {
                return std::regex_replace(std::string(input), ansi_regex(), "");
            }

            // ----- Has ANSI
            [[nodiscard]] inline bool has_ansi(std::string_view input) {
                return std::regex_search(input.begin(), input.end(), ansi_regex());
            }

        } // namespace trim

        [[nodiscard]] inline std::string replace(std::string_view str, std::string_view from, std::string_view to) {
            if (from.empty())
                return std::string(str);
            const std::size_t pos = str.find(from);
            if (pos == std::string_view::npos)
                return std::string(str);

            std::string out;
            out.reserve(str.size() - from.size() + to.size());
            out.append(str.substr(0, pos));
            out.append(to);
            out.append(str.substr(pos + from.size()));
            return out;
        }

        [[nodiscard]] inline std::string replace(std::string_view str, std::string_view from, char to) {
            return replace(str, from, std::string_view(&to, 1));
        }

        [[nodiscard]] inline std::string replace_all(std::string_view str, std::string_view from, std::string_view to) {
            if (from.empty())
                return std::string(str);

            std::string out;
            out.reserve(str.size());
            std::size_t start = 0;
            while (true) {
                const std::size_t pos = str.find(from, start);
                if (pos == std::string_view::npos) {
                    out.append(str.substr(start));
                    break;
                }
                out.append(str.substr(start, pos - start));
                out.append(to);
                start = pos + from.size();
            }
            return out;
        }

        [[nodiscard]] inline std::string replace_all(std::string_view str, std::string_view from, char to) {
            return replace_all(str, from, std::string_view(&to, 1));
        }

        [[nodiscard]] inline std::string replace_first_of(std::string_view str, std::string_view chars, std::string_view to) {
            const std::size_t pos = str.find_first_of(chars);
            if (pos == std::string_view::npos)
                return std::string(str);

            std::string out;
            out.reserve(str.size() - 1 + to.size());
            out.append(str.substr(0, pos));
            out.append(to);
            out.append(str.substr(pos + 1));
            return out;
        }

        [[nodiscard]] inline std::string replace_first_of(std::string_view str, std::string_view chars, char to) {
            return replace_first_of(str, chars, std::string_view(&to, 1));
        }

        [[nodiscard]] inline std::string replace_last_of(std::string_view str, std::string_view chars, std::string_view to) {
            const std::size_t pos = str.find_last_of(chars);
            if (pos == std::string_view::npos)
                return std::string(str);

            std::string out;
            out.reserve(str.size() - 1 + to.size());
            out.append(str.substr(0, pos));
            out.append(to);
            out.append(str.substr(pos + 1));
            return out;
        }

        [[nodiscard]] inline std::string replace_last_of(std::string_view str, std::string_view chars, char to) {
            return replace_last_of(str, chars, std::string_view(&to, 1));
        }

    } // namespace str

    // ----- Regular title function
    inline void title(const std::string &t, int w = 40) {
        std::string sep(w, '=');
        std::cout << std::format("{0}\n{1}\n{0}\n", sep, t) << std::flush;
    }

    // ----- Works the same as "std::string(int, char)" but maded to use string instead
    inline std::string repeatString(const std::string &str, int count) {
        std::string result;
        result.reserve(str.size() * static_cast<std::size_t>(count > 0 ? count : 0));
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
        const int msgLen = static_cast<int>(msg.length()) + 2;
        const std::size_t totalWidth = static_cast<std::size_t>(consoleWidth);
        const int sideLen = (consoleWidth - msgLen) / 2;

        std::string result;
        result.reserve(totalWidth);
        result.append(static_cast<std::size_t>(sideLen > 0 ? sideLen : 0), a);
        result += " " + msg + " ";
        result.append(static_cast<std::size_t>(sideLen > 0 ? sideLen : 0), a);
        if (result.length() < totalWidth) {
            result += a;
        }
        std::cout << "\r\033[2K";
        std::cout << result << '\n';
    }

    // ----- Wait for enter key to be pressed
    inline void waitForEnter(std::function<void()> func = nullptr) {
        std::cout << "Press Enter To Continue..." << std::endl;
        std::cout.flush();
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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