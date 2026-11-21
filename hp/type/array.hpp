#pragma once

#include <algorithm>
#include <compare>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace hp {
    template <typename T, std::unsigned_integral auto N>
    class array {
        static_assert(N > 0, "hp::array capacity N must be greater than 0");

      public:
        // ----- Member Types
        using value_type = T;
        using size_type = decltype(N);
        using difference_type = std::ptrdiff_t;
        using reference = T &;
        using const_reference = const T &;
        using pointer = T *;
        using const_pointer = const T *;
        using iterator = T *;
        using const_iterator = const T *;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

      private:
        T data_[N]{};
        size_type size_ = 0;

      public:
        // ----- Constructors & Destructor
        constexpr array() = default;
        constexpr array(const array &) = default;
        constexpr array(array &&) noexcept(std::is_nothrow_move_constructible_v<T>) = default;
        constexpr array &operator=(const array &) = default;
        constexpr array &operator=(array &&) noexcept(std::is_nothrow_move_assignable_v<T>) = default;
        constexpr ~array() = default;

        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, T>
        constexpr array(std::from_range_t, R &&rg) {
            append_range(std::forward<R>(rg));
        }

        template <std::ranges::input_range R>
            requires(!std::same_as<std::remove_cvref_t<R>, array>) && std::convertible_to<std::ranges::range_reference_t<R>, T>
        constexpr array(R &&init) {
            if constexpr (std::ranges::sized_range<R>) {
                if (std::cmp_greater(std::ranges::size(init), N))
                    throw std::length_error("hp::array: container exceeds capacity");

                std::ranges::copy(init, data_);
                size_ = static_cast<size_type>(std::ranges::size(init));
            } else {
                for (auto &&elem : init) {
                    if (size_ >= N)
                        throw std::length_error("hp::array: container exceeds capacity");

                    data_[size_++] = std::forward<decltype(elem)>(elem);
                }
            }
        }

        // ----- Size & Capacity
        template <std::unsigned_integral Size = size_type>
        [[nodiscard]] constexpr Size size() const noexcept {
            return static_cast<Size>(size_);
        }

        template <std::unsigned_integral Size = size_type>
        [[nodiscard]] constexpr Size capacity() const noexcept {
            return static_cast<Size>(N);
        }

        [[nodiscard]] constexpr bool empty() const noexcept {
            return size_ == 0;
        }

        [[nodiscard]] constexpr bool full() const noexcept {
            return N == size_;
        }

        constexpr void resize(std::unsigned_integral auto new_size, const T &value = T{}) {
            if (std::cmp_greater(new_size, N))
                throw std::length_error("hp::array::resize: new_size exceeds capacity");

            const auto target = static_cast<size_type>(new_size);
            if (target > size_) {
                std::ranges::fill(data_ + size_, data_ + target, value);
            } else if (target < size_) {
                std::ranges::fill(data_ + target, data_ + size_, T{});
            }
            size_ = target;
        }

        // ----- Element Access
        [[nodiscard]] constexpr T &operator[](std::unsigned_integral auto i) noexcept {
            return data_[i];
        }

        [[nodiscard]] constexpr const T &operator[](std::unsigned_integral auto i) const noexcept {
            return data_[i];
        }

        [[nodiscard]] constexpr T &at(std::unsigned_integral auto i) {
            if (std::cmp_greater_equal(i, size_))
                throw std::out_of_range("hp::array::at: index out of range");

            return data_[i];
        }

        [[nodiscard]] constexpr const T &at(std::unsigned_integral auto i) const {
            if (std::cmp_greater_equal(i, size_))
                throw std::out_of_range("hp::array::at: index out of range");

            return data_[i];
        }

        [[nodiscard]] constexpr T &front() noexcept {
            return data_;
        }
        [[nodiscard]] constexpr const T &front() const noexcept {
            return data_;
        }

        [[nodiscard]] constexpr T &back() noexcept {
            return data_[size_ - 1];
        }
        [[nodiscard]] constexpr const T &back() const noexcept {
            return data_[size_ - 1];
        }

        [[nodiscard]] constexpr T *data() noexcept {
            return data_;
        }
        [[nodiscard]] constexpr const T *data() const noexcept {
            return data_;
        }

        // ----- Iterators
        [[nodiscard]] constexpr iterator begin() noexcept {
            return data_;
        }
        [[nodiscard]] constexpr const_iterator begin() const noexcept {
            return data_;
        }

        [[nodiscard]] constexpr iterator end() noexcept {
            return data_ + size_;
        }
        [[nodiscard]] constexpr const_iterator end() const noexcept {
            return data_ + size_;
        }

        [[nodiscard]] constexpr reverse_iterator rbegin() noexcept {
            return reverse_iterator(end());
        }
        [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(end());
        }

        [[nodiscard]] constexpr reverse_iterator rend() noexcept {
            return reverse_iterator(begin());
        }
        [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(begin());
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
            return begin();
        }
        [[nodiscard]] constexpr const_iterator cend() const noexcept {
            return end();
        }
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }
        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept {
            return rend();
        }

        // ----- Modifiers
        constexpr T &push_back(const T &value) {
            if (size_ >= N)
                throw std::out_of_range("hp::array::push_back: array is full");

            data_[size_] = value;
            return data_[size_++];
        }

        constexpr T &push_back(T &&value) {
            if (size_ >= N)
                throw std::out_of_range("hp::array::push_back: array is full");

            data_[size_] = std::move(value);
            return data_[size_++];
        }

        template <typename... Args>
        constexpr T &emplace_back(Args &&...args) {
            if (size_ >= N)
                throw std::out_of_range("hp::array::emplace_back: array is full");

            data_[size_] = T(std::forward<Args>(args)...);
            return data_[size_++];
        }

        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, T>
        constexpr void append_range(R &&rg) {
            if constexpr (std::ranges::sized_range<R>) {
                if (std::cmp_greater(std::ranges::size(rg), N - size_))
                    throw std::length_error("hp::array::append_range: exceeds capacity");
            }

            for (auto &&elem : rg) {
                if (size_ >= N)
                    throw std::length_error("hp::array::append_range: exceeds capacity");

                data_[size_++] = std::forward<decltype(elem)>(elem);
            }
        }

        constexpr T &push_front(const T &value) {
            if (size_ >= N)
                throw std::out_of_range("hp::array::push_front: array is full");

            std::ranges::move_backward(data_, data_ + size_, data_ + size_ + 1);
            data_ = value;
            ++size_;
            return data_;
        }

        constexpr T &push_front(T &&value) {
            if (size_ >= N)
                throw std::out_of_range("hp::array::push_front: array is full");

            std::ranges::move_backward(data_, data_ + size_, data_ + size_ + 1);
            data_ = std::move(value);
            ++size_;
            return data_;
        }

        constexpr iterator erase(std::unsigned_integral auto idx) {
            if (std::cmp_greater_equal(idx, size_))
                throw std::out_of_range("hp::array::erase: index out of range");

            std::ranges::move(data_ + idx + 1, data_ + size_, data_ + idx);
            --size_;
            data_[size_] = T{};
            return data_ + idx;
        }

        constexpr T &insert(std::unsigned_integral auto idx, const T &value) {
            if (size_ >= N)
                throw std::out_of_range("hp::array::insert: capacity exceeded");
            if (std::cmp_greater(idx, size_))
                throw std::out_of_range("hp::array::insert: index out of range");

            std::ranges::move_backward(data_ + idx, data_ + size_, data_ + size_ + 1);
            data_[idx] = value;
            ++size_;
            return data_[idx];
        }

        constexpr T &insert(std::unsigned_integral auto idx, T &&value) {
            if (size_ >= N)
                throw std::out_of_range("hp::array::insert: capacity exceeded");
            if (std::cmp_greater(idx, size_))
                throw std::out_of_range("hp::array::insert: index out of range");

            std::ranges::move_backward(data_ + idx, data_ + size_, data_ + size_ + 1);
            data_[idx] = std::move(value);
            ++size_;
            return data_[idx];
        }

        constexpr void clear() noexcept {
            std::ranges::fill(data_, data_ + size_, T{});
            size_ = 0;
        }

        // ----- Comparisons
        [[nodiscard]] constexpr bool operator==(const array &other) const
            requires std::equality_comparable<T>
        {
            return std::ranges::equal(*this, other);
        }

        [[nodiscard]] constexpr auto operator<=>(const array &other) const
            requires std::three_way_comparable<T>
        {
            return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
        }
    };

} // namespace hp
