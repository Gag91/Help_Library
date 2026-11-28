#pragma once

#include "hp/print/print.hpp"
#include <expected>
#include <format>
#include <source_location>
#include <string>
#include <string_view>

namespace hp {

    enum class error_code {
        none = 0,
        invalid_argument,
        not_found,
        permission_denied,
        io_failure,
        timeout,
        internal
    };

    struct error {
        error_code code{error_code::none};
        std::string message;
        std::source_location location = std::source_location::current();

        constexpr error() noexcept = default;
        constexpr error(error_code c, std::string_view msg, std::source_location loc = std::source_location::current()) : code(c), message(msg), location(loc) {}

        template <typename... Args>
        static error make(error_code c, std::format_string<Args...> fmt, Args &&...args) {
            return error{c, std::format(fmt, std::forward<Args>(args)...)};
        }

        [[nodiscard]] constexpr explicit operator bool() const noexcept {
            return code != error_code::none;
        }

        [[nodiscard]] std::string format() const {
            if (!*this)
                return "ok";
            return std::format("[{}:{}] (code {}): {}", location.file_name(), location.line(), static_cast<int>(code), message);
        }
    };

    template <typename T>
    using result = std::expected<T, error>;

    [[nodiscard]] inline std::unexpected<error> fail(error_code code, std::string_view msg = "", std::source_location loc = std::source_location::current()) {
        return std::unexpected(error{code, msg, loc});
    }

    // ----- Shorthands
    [[nodiscard]] inline std::unexpected<error> invalid_arg(std::string_view msg = "", std::source_location loc = std::source_location::current()) {
        return fail(error_code::invalid_argument, msg, loc);
    }

    [[nodiscard]] inline std::unexpected<error> not_found(std::string_view msg = "", std::source_location loc = std::source_location::current()) {
        return fail(error_code::not_found, msg, loc);
    }

    [[nodiscard]] inline std::unexpected<error> permission_denied(std::string_view msg = "", std::source_location loc = std::source_location::current()) {
        return fail(error_code::permission_denied, msg, loc);
    }

    [[nodiscard]] inline std::unexpected<error> io_failure(std::string_view msg = "", std::source_location loc = std::source_location::current()) {
        return fail(error_code::io_failure, msg, loc);
    }

    [[nodiscard]] inline std::unexpected<error> timeout(std::string_view msg = "", std::source_location loc = std::source_location::current()) {
        return fail(error_code::timeout, msg, loc);
    }

    [[nodiscard]] inline std::unexpected<error> internal_error(std::string_view msg = "", std::source_location loc = std::source_location::current()) {
        return fail(error_code::internal, msg, loc);
    }

} // namespace hp