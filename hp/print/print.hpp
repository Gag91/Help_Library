#include "hp/string/string.hpp"

#include <cctype>
#include <concepts>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <ostream>

namespace hp::detail {

    class OStream {
      public:
        explicit OStream(std::ostream &os) noexcept : os_(os) {}

        OStream &operator<<(std::ostream &(*m)(std::ostream &)) {
            os_ << m;
            return *this;
        }
        OStream &operator<<(std::ios_base &(*m)(std::ios_base &)) {
            os_ << m;
            return *this;
        }

        template <typename T>
        OStream &operator<<(const T &value) {
            if constexpr (requires(std::ostream &o, const T &v) { o << v; }) {
                os_ << value;
            } else {
                os_ << hp::str::to_string(value);
            }
            return *this;
        }

        std::ostream &native() noexcept {
            return os_;
        }

      private:
        std::ostream &os_;
    };

    // ----- Count Placeholders
    consteval std::size_t count_placeholders(std::string_view fmt) {
        std::size_t count = 0;
        for (std::size_t i = 0; i < fmt.size(); ++i) {
            if (fmt[i] == '{') {
                if (i + 1 < fmt.size() && fmt[i + 1] == '{') {
                    ++i;
                    continue;
                }
                std::size_t close = fmt.find('}', i + 1);
                if (close == std::string_view::npos) {
                    throw std::runtime_error("Unclosed '{' in format string");
                }
                ++count;
                i = close;
            } else if (fmt[i] == '}') {
                if (i + 1 < fmt.size() && fmt[i + 1] == '}') {
                    ++i;
                    continue;
                }
                throw std::runtime_error("Unmatched '}' in format string");
            }
        }
        return count;
    }

    template <typename... Args>
    struct basic_format_string {
        std::string_view str;

        // ----- Validate Format String
        template <typename T>
            requires std::convertible_to<const T &, std::string_view>
        consteval basic_format_string(const T &s) : str(s) {
            if (count_placeholders(str) != sizeof...(Args)) {
                throw std::runtime_error("Argument count does not match format placeholders");
            }
        }
    };

    template <typename... Args>
    using format_string = basic_format_string<std::type_identity_t<Args>...>;

} // namespace hp::detail

namespace hp {

    // ----- hp::cout, cerr, clog
    inline detail::OStream cout{std::cout};
    inline detail::OStream cerr{std::cerr};
    inline detail::OStream clog{std::clog};

    // ----- Format String
    template <typename... Args>
    std::string format(hp::detail::format_string<Args...> fmt, const Args &...args) {
        std::string_view value = fmt.str;
        std::vector<std::string> arguments;
        arguments.reserve(sizeof...(Args));
        (arguments.push_back(hp::str::to_string(args)), ...);

        std::string result;
        std::size_t argument = 0;

        for (std::size_t i = 0; i < value.size(); ++i) {

            if (value[i] == '{' && i + 1 < value.size() && value[i + 1] == '{') {
                result += '{';
                ++i;
                continue;
            }

            if (value[i] == '}' && i + 1 < value.size() && value[i + 1] == '}') {
                result += '}';
                ++i;
                continue;
            }

            if (value[i] == '{' && i + 1 < value.size() && value[i + 1] == '}' && argument < arguments.size()) {
                result += arguments[argument++];
                ++i;
                continue;
            }

            if (value[i] == '{' && i + 3 < value.size() && value[i + 1] == ':' && value[i + 2] == '.') {
                std::size_t j = i + 3;
                int prec = 0;

                while (j < value.size() && std::isdigit(static_cast<unsigned char>(value[j]))) {
                    prec = prec * 10 + (value[j] - '0');
                    ++j;
                }

                if (j < value.size() && value[j] == 'f') {
                    ++j;
                }

                if (j < value.size() && value[j] == '}' && argument < arguments.size()) {
                    const std::string &arg_str = arguments[argument++];
                    try {
                        double val = std::stod(arg_str);
                        std::ostringstream oss;
                        oss << std::fixed << std::setprecision(prec) << val;
                        result += oss.str();
                    } catch (...) {
                        result += arg_str;
                    }
                    i = j;
                    continue;
                }
            }

            if (value[i] == '{' && i + 2 < value.size() && value[i + 1] == ':') {
                char align = '<';
                std::size_t j = i + 2;

                if (value[j] == '<' || value[j] == '>' || value[j] == '^') {
                    align = value[j];
                    ++j;
                }

                std::size_t width = 0;
                while (j < value.size() && std::isdigit(static_cast<unsigned char>(value[j]))) {
                    width = width * 10 + (value[j] - '0');
                    ++j;
                }

                if (j < value.size() && value[j] == '}' && argument < arguments.size()) {
                    const std::string &arg_str = arguments[argument++];
                    std::size_t padding = (width > arg_str.size()) ? (width - arg_str.size()) : 0;

                    if (align == '<') {
                        result += arg_str;
                        result.append(padding, ' ');
                    } else if (align == '>') {
                        result.append(padding, ' ');
                        result += arg_str;
                    } else if (align == '^') {
                        std::size_t left_pad = padding / 2;
                        std::size_t right_pad = padding - left_pad;
                        result.append(left_pad, ' ');
                        result += arg_str;
                        result.append(right_pad, ' ');
                    }

                    i = j;
                    continue;
                }
            }

            if (value[i] == '{') {
                throw std::runtime_error("Unsupported or malformed format specifier");
            }

            result += value[i];
        }

        return result;
    }

    // ----- Print Formatted String
    template <typename... Args>
    void print(hp::detail::format_string<Args...> fmt, const Args &...args) {
        std::cout << hp::format(fmt, args...);
    }

    // ----- Print Formatted String With Newline
    template <typename... Args>
    void println(hp::detail::format_string<Args...> fmt, const Args &...args) {
        std::cout << hp::format(fmt, args...) << '\n';
    }

} // namespace hp