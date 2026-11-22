#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <utility>

namespace hp {
    // ----- Size Aliases
    using uz = decltype(0uz);
    using iz = decltype(0z);

    // ----- Fixed-Width Integer Aliases
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using u128 = unsigned __int128;
    using i128 = __int128;

    using byte = std::byte;

    // ----- Floating-Point Aliases
    using f32 = float;
    using f64 = double;

    // ----- Safe Numeric Casts (Catches narrowing/sign bugs!)
    template <std::integral To, std::integral From>
    [[nodiscard]] constexpr To narrow_cast(From value) {
        if (!std::in_range<To>(value)) {
            throw std::out_of_range("hp::narrow_cast: value out of target range");
        }

        return static_cast<To>(value);
    }

    // ----- User-Defined Literals
    inline namespace literals {
        [[nodiscard]] consteval u8 operator""_u8(unsigned long long v) {
            return narrow_cast<u8>(v);
        }
        [[nodiscard]] consteval u16 operator""_u16(unsigned long long v) {
            return narrow_cast<u16>(v);
        }
        [[nodiscard]] consteval u32 operator""_u32(unsigned long long v) {
            return narrow_cast<u32>(v);
        }
        [[nodiscard]] consteval u64 operator""_u64(unsigned long long v) {
            return narrow_cast<u64>(v);
        }
        [[nodiscard]] consteval i8 operator""_i8(unsigned long long v) {
            return narrow_cast<i8>(v);
        }
        [[nodiscard]] consteval i16 operator""_i16(unsigned long long v) {
            return narrow_cast<i16>(v);
        }
        [[nodiscard]] consteval i32 operator""_i32(unsigned long long v) {
            return narrow_cast<i32>(v);
        }
        [[nodiscard]] consteval i64 operator""_i64(unsigned long long v) {
            return narrow_cast<i64>(v);
        }
    } // namespace literals

} // namespace hp
