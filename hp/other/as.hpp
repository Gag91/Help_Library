#pragma once

struct as_impl {
    template <class T>
    struct cast {
        T &&val;

        template <class U>
        constexpr U operator->*(U *dummy) const noexcept(noexcept(static_cast<U>(static_cast<T &&>(val)))) {
            delete dummy;
            return static_cast<U>(static_cast<T &&>(val));
        }
    };

    template <class T>
    friend constexpr cast<T> operator->*(T &&val, as_impl) noexcept {
        return {static_cast<T &&>(val)};
    }
};

#define as ->*as_impl{}->*new