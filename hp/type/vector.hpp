#pragma once

#include <algorithm>
#include <compare>
#include <concepts>
#include <cstddef>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <utility>

namespace hp {

    template <typename T, typename allocator = std::allocator<T>>
    class vector {
      public:
        // ----- Member Types
        using value_type = T;
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
        using alloc_traits = std::allocator_traits<allocator>;

        T *data_ = nullptr;
        std::size_t size_ = 0;
        std::size_t capacity_ = 0;
        [[no_unique_address]] allocator alloc_{};

        constexpr void destroy_elements(T *first, T *last) noexcept {
            for (; first != last; ++first) {
                alloc_traits::destroy(alloc_, first);
            }
        }

        constexpr void destroy_and_deallocate() noexcept {
            if (data_) {
                destroy_elements(data_, data_ + size_);
                alloc_traits::deallocate(alloc_, data_, capacity_);
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
            }
        }

      public:
        // ----- Constructors & Destructor
        constexpr vector() = default;
        constexpr explicit vector(const allocator &alloc) noexcept : alloc_(alloc) {}
        template <typename alloc_arg>
            requires(!std::same_as<std::remove_cvref_t<alloc_arg>, allocator>) && std::constructible_from<allocator, alloc_arg &&>
        constexpr explicit vector(alloc_arg &&alloc) : alloc_(std::forward<alloc_arg>(alloc)) {}
        constexpr vector(const vector &other) : alloc_(other.alloc_) {
            if (other.capacity_ != 0) {
                data_ = alloc_traits::allocate(alloc_, other.capacity_);
                for (std::size_t i = 0; i < other.size_; ++i) {
                    alloc_traits::construct(alloc_, data_ + i, other.data_[i]);
                }
            }
            size_ = other.size_;
            capacity_ = other.capacity_;
        }

        constexpr vector(vector &&other) noexcept
            : data_(other.data_), size_(other.size_), capacity_(other.capacity_),
              alloc_(std::move(other.alloc_)) {
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        constexpr vector &operator=(const vector &other) {
            if (this != &other) {
                vector tmp(other);
                destroy_and_deallocate();
                data_ = tmp.data_;
                size_ = tmp.size_;
                capacity_ = tmp.capacity_;
                alloc_ = tmp.alloc_;
                tmp.data_ = nullptr;
                tmp.size_ = 0;
                tmp.capacity_ = 0;
            }
            return *this;
        }

        constexpr vector &operator=(vector &&other) noexcept {
            if (this != &other) {
                destroy_and_deallocate();
                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;
                alloc_ = std::move(other.alloc_);
                other.data_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            }
            return *this;
        }

        constexpr ~vector() {
            if (data_) {
                for (std::size_t i = 0; i < size_; ++i) {
                    alloc_traits::destroy(alloc_, data_ + i);
                }
                alloc_traits::deallocate(alloc_, data_, capacity_);
            }
        }

        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, T>
        constexpr vector(std::from_range_t, R &&rg) {
            append_range(std::forward<R>(rg));
        }

        constexpr vector(std::initializer_list<T> init) {
            append_range(init);
        }

        template <std::ranges::input_range R>
            requires(!std::same_as<std::remove_cvref_t<R>, vector>) &&
                    std::convertible_to<std::ranges::range_reference_t<R>, T>
        constexpr explicit vector(R &&init, const allocator &alloc = allocator())
            : alloc_(alloc) {
            append_range(std::forward<R>(init));
        }

        // ----- Size & Capacity
        template <std::unsigned_integral Size = std::size_t>
        [[nodiscard]] constexpr Size size() const noexcept {
            return size_;
        }

        template <std::unsigned_integral Size = std::size_t>
        [[nodiscard]] constexpr Size capacity() const noexcept {
            return capacity_;
        }

        [[nodiscard]] constexpr bool empty() const noexcept {
            return size_ == 0;
        }

        constexpr void reserve(std::unsigned_integral auto new_cap) {
            const std::size_t target = static_cast<std::size_t>(new_cap);
            if (target <= capacity_)
                return;

            T *new_data = alloc_traits::allocate(alloc_, target);
            if (data_) {
                for (std::size_t i = 0; i < size_; ++i) {
                    alloc_traits::construct(alloc_, new_data + i, std::move(data_[i]));
                }
                for (std::size_t i = 0; i < size_; ++i) {
                    alloc_traits::destroy(alloc_, data_ + i);
                }
                alloc_traits::deallocate(alloc_, data_, capacity_);
            }

            data_ = new_data;
            capacity_ = target;
        }
        constexpr void reserve(std::signed_integral auto) = delete;

        // ----- Element Access
        [[nodiscard]] constexpr T &operator[](std::unsigned_integral auto i) noexcept {
            return data_[i];
        }
        [[nodiscard]] constexpr T &operator[](std::signed_integral auto) = delete;

        [[nodiscard]] constexpr const T &operator[](std::unsigned_integral auto i) const noexcept {
            return data_[i];
        }
        [[nodiscard]] constexpr const T &operator[](std::signed_integral auto) const = delete;

        [[nodiscard]] constexpr T &at(std::unsigned_integral auto i) {
            if (std::cmp_greater_equal(i, size_))
                throw std::out_of_range("hp::vector::at: index out of range");

            return data_[i];
        }
        [[nodiscard]] constexpr T &at(std::signed_integral auto) = delete;

        [[nodiscard]] constexpr const T &at(std::unsigned_integral auto i) const {
            if (std::cmp_greater_equal(i, size_))
                throw std::out_of_range("hp::vector::at: index out of range");

            return data_[i];
        }

        [[nodiscard]] constexpr T &front() noexcept {
            return data_[0];
        }
        [[nodiscard]] constexpr const T &front() const noexcept {
            return data_[0];
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
        template <typename... Args>
        constexpr T &emplace_back(Args &&...args) {
            if (size_ == capacity_)
                reserve(capacity_ == 0 ? std::size_t{4} : capacity_ * std::size_t{2});

            alloc_traits::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
            return data_[size_++];
        }

        constexpr T &push_back(const T &value) {
            return emplace_back(value);
        }
        constexpr T &push_back(T &&value) {
            return emplace_back(std::move(value));
        }

        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, T>
        constexpr void append_range(R &&rg) {
            if constexpr (std::ranges::sized_range<R>) {
                const auto extra = static_cast<std::size_t>(std::ranges::size(rg));
                if (size_ + extra > capacity_) {
                    reserve(std::max(capacity_ * std::size_t{2}, size_ + extra));
                }
            }
            for (auto &&elem : rg) {
                emplace_back(std::forward<decltype(elem)>(elem));
            }
        }

        constexpr T &push_front(const T &value) {
            if (size_ == capacity_)
                reserve(capacity_ == 0 ? std::size_t{4} : capacity_ * std::size_t{2});
            if (size_ != 0)
                alloc_traits::construct(alloc_, data_ + size_, std::move(data_[size_ - 1]));
            if (size_ != 0)
                std::ranges::move_backward(data_, data_ + size_ - 1, data_ + size_);
            if (size_ == 0)
                alloc_traits::construct(alloc_, data_, value);
            else
                data_[0] = value;
            ++size_;
            return data_[0];
        }

        constexpr T &push_front(T &&value) {
            if (size_ == capacity_)
                reserve(capacity_ == 0 ? std::size_t{4} : capacity_ * std::size_t{2});
            if (size_ != 0)
                alloc_traits::construct(alloc_, data_ + size_, std::move(data_[size_ - 1]));
            if (size_ != 0)
                std::ranges::move_backward(data_, data_ + size_ - 1, data_ + size_);
            if (size_ == 0)
                alloc_traits::construct(alloc_, data_, std::move(value));
            else
                data_[0] = std::move(value);
            ++size_;
            return data_[0];
        }

        constexpr iterator erase(std::unsigned_integral auto idx) {
            if (std::cmp_greater_equal(idx, size_))
                throw std::out_of_range("hp::vector::erase: index out of range");

            std::ranges::move(data_ + idx + 1, data_ + size_, data_ + idx);
            --size_;
            alloc_traits::destroy(alloc_, data_ + size_);
            return data_ + idx;
        }
        constexpr iterator erase(std::signed_integral auto) = delete;

        constexpr T &insert(std::unsigned_integral auto idx, const T &value) {
            if (std::cmp_greater(idx, size_))
                throw std::out_of_range("hp::vector::insert: index out of range");

            const auto pos = static_cast<std::size_t>(idx);
            if (pos == size_)
                return emplace_back(value);

            if (size_ == capacity_)
                reserve(capacity_ == 0 ? std::size_t{4} : capacity_ * std::size_t{2});

            alloc_traits::construct(alloc_, data_ + size_, std::move(data_[size_ - 1]));
            std::ranges::move_backward(data_ + pos, data_ + size_ - 1, data_ + size_);
            data_[pos] = value;
            ++size_;
            return data_[pos];
        }
        constexpr T &insert(std::signed_integral auto, const T &) = delete;

        constexpr T &insert(std::unsigned_integral auto idx, T &&value) {
            if (std::cmp_greater(idx, size_))
                throw std::out_of_range("hp::vector::insert: index out of range");

            const std::size_t pos = static_cast<std::size_t>(idx);
            if (pos == size_)
                return emplace_back(std::move(value));

            if (size_ == capacity_)
                reserve(capacity_ == 0 ? std::size_t{4} : capacity_ * std::size_t{2});

            alloc_traits::construct(alloc_, data_ + size_, std::move(data_[size_ - 1]));
            std::ranges::move_backward(data_ + pos, data_ + size_ - 1, data_ + size_);
            data_[pos] = std::move(value);
            ++size_;
            return data_[pos];
        }
        constexpr void clear() noexcept {
            for (std::size_t i = 0; i < size_; ++i) {
                alloc_traits::destroy(alloc_, data_ + i);
            }
            size_ = 0;
        }

        constexpr void swap(vector &other) noexcept {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
        }

        // ----- Comparisons
        [[nodiscard]] constexpr bool operator==(const vector &other) const
            requires std::equality_comparable<T>
        {
            return std::ranges::equal(*this, other);
        }

        [[nodiscard]] constexpr auto operator<=>(const vector &other) const
            requires std::three_way_comparable<T>
        {
            return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
        }
    };

    template <std::ranges::input_range R>
    vector(R &&) -> vector<std::ranges::range_value_t<R>>;
} // namespace hp