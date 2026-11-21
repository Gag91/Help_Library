#pragma once

#include <algorithm>
#include <compare>
#include <cstddef>
#include <format>
#include <iterator>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace hp {

    class string {
      private:
        std::size_t size_ = 0;
        std::size_t capacity_ = 0;
        char *data_ = const_cast<char *>("");
        bool owned_ = false;

        constexpr void release() noexcept {
            if (owned_) {
                delete[] data_;
            }
            data_ = const_cast<char *>("");
            size_ = 0;
            capacity_ = 0;
            owned_ = false;
        }

        constexpr void replace(const char *value, std::size_t count) {
            if (count == 0) {
                release();
                return;
            }

            auto *new_data = new char[count + 1];
            std::char_traits<char>::copy(new_data, value, count);
            new_data[count] = '\0';

            if (owned_) {
                delete[] data_;
            }
            data_ = new_data;
            size_ = count;
            capacity_ = count;
            owned_ = true;
        }

        constexpr void ensure_capacity(std::size_t required) {
            if (required <= capacity_) {
                return;
            }

            if (required > npos - 1) {
                throw std::length_error("hp::string: size exceeds maximum");
            }

            std::size_t new_capacity = capacity_ == 0 ? required : capacity_;
            while (new_capacity < required) {
                if (new_capacity > npos / 2) {
                    new_capacity = required;
                    break;
                }
                new_capacity *= 2;
            }

            auto *new_data = new char[new_capacity + 1];
            if (size_ > 0) {
                std::char_traits<char>::copy(new_data, data_, size_);
            }
            new_data[size_] = '\0';

            if (owned_) {
                delete[] data_;
            }
            data_ = new_data;
            capacity_ = new_capacity;
            owned_ = true;
        }

      public:
        static constexpr std::size_t npos = static_cast<std::size_t>(-1);

        // ----- Constructors & Destructor
        constexpr string() = default;
        constexpr string(const string &other) {
            if (!other.empty()) {
                replace(other.data_, other.size_);
            }
        }

        constexpr string(string &&other) noexcept
            : size_(other.size_), capacity_(other.capacity_), data_(other.data_), owned_(other.owned_) {
            other.data_ = const_cast<char *>("");
            other.size_ = 0;
            other.capacity_ = 0;
            other.owned_ = false;
        }

        constexpr string(const char *value) {
            if (value) {
                replace(value, std::char_traits<char>::length(value));
            }
        }

        constexpr string(const char *value, std::size_t count) {
            if (value && count > 0) {
                replace(value, count);
            }
        }

        constexpr string(const std::string &value) {
            if (!value.empty()) {
                replace(value.data(), value.size());
            }
        }

        constexpr string(std::string_view value) {
            if (!value.empty()) {
                replace(value.data(), value.size());
            }
        }

        constexpr ~string() {
            if (owned_) {
                delete[] data_;
            }
        }

        // ----- Assignment
        constexpr string &operator=(const string &other) {
            if (this != &other) {
                if (other.empty()) {
                    release();
                } else {
                    replace(other.data_, other.size_);
                }
            }
            return *this;
        }

        constexpr string &operator=(string &&other) noexcept {
            if (this != &other) {
                if (owned_) {
                    delete[] data_;
                }
                size_ = other.size_;
                capacity_ = other.capacity_;
                data_ = other.data_;
                owned_ = other.owned_;
                other.data_ = const_cast<char *>("");
                other.size_ = 0;
                other.capacity_ = 0;
                other.owned_ = false;
            }
            return *this;
        }

        // ----- Append string
        constexpr string &operator+=(const string &other) {
            if (other.empty())
                return *this;

            if (other.size_ > npos - size_)
                throw std::length_error("hp::string: size exceeds maximum");

            ensure_capacity(size_ + other.size_);
            std::char_traits<char>::copy(data_ + size_, other.data_, other.size_);
            size_ += other.size_;
            data_[size_] = '\0';
            return *this;
        }

        // ----- Append std::string
        constexpr string &operator+=(const std::string &other) {
            if (other.empty())
                return *this;

            if (other.size() > npos - size_)
                throw std::length_error("hp::string: size exceeds maximum");

            ensure_capacity(size_ + other.size());
            std::char_traits<char>::copy(data_ + size_, other.data(), other.size());
            size_ += other.size();
            data_[size_] = '\0';
            return *this;
        }

        void push_back(std::string str) {
            *this += str;
        }

        // ----- Append std::string_view
        constexpr string &operator+=(const std::string_view &other) {
            if (other.empty())
                return *this;

            if (other.size() > npos - size_)
                throw std::length_error("hp::string: size exceeds maximum");

            ensure_capacity(size_ + other.size());
            std::char_traits<char>::copy(data_ + size_, other.data(), other.size());
            size_ += other.size();
            data_[size_] = '\0';
            return *this;
        }

        void push_back(std::string_view sv) {
            *this += sv;
        }

        // ----- Append char
        constexpr string &operator+=(const char &other) {
            if (other == '\0')
                return *this;

            const std::size_t new_size = size_ + 1;
            if (new_size < size_ || new_size > npos - 1) {
                throw std::length_error("hp::string: size exceeds maximum");
            }
            auto *new_data = new char[new_size + 1];

            std::char_traits<char>::copy(new_data, data_, size_);
            new_data[size_] = other;
            new_data[new_size] = '\0';

            if (owned_) {
                delete[] data_;
            }
            data_ = new_data;
            size_ = new_size;
            capacity_ = new_size;
            owned_ = true;
            return *this;
        }

        void push_back(char c) {
            *this += c;
        }

        // ----- Append functions
        string &append(const string &s) {
            return *this += s;
        }
        string &append(std::string_view sv) {
            return *this += sv;
        }
        string &append(const char *s) {
            return *this += string(s);
        }
        string &append(std::string const &s) {
            return *this += s;
        }

        // ----- Capacity
        constexpr std::size_t max_size() const noexcept {
            return npos / sizeof(char) - 1;
        }

        constexpr std::size_t size() const noexcept {
            return size_;
        }
        constexpr std::size_t length() const noexcept {
            return size_;
        }
        constexpr bool empty() const noexcept {
            return size_ == 0;
        }

        // ----- Element Access
        constexpr char *c_str() const noexcept {
            return data_;
        }

        constexpr const char *data() const noexcept {
            return data_;
        }
        constexpr const char &operator[](std::size_t pos) const noexcept {
            return data_[pos];
        }
        constexpr const char &front() const noexcept {
            return data_[0];
        }
        constexpr const char &back() const noexcept {
            return data_[size_ - 1];
        }

        constexpr const char &at(std::size_t pos) const {
            if (pos >= size_) {
                throw std::out_of_range("hp::string::at: index out of range");
            }
            return data_[pos];
        }

        // ----- Iterators
        constexpr const char *begin() const noexcept {
            return data_;
        }
        constexpr const char *cbegin() const noexcept {
            return data_;
        }
        constexpr const char *end() const noexcept {
            return data_ + size_;
        }
        constexpr const char *cend() const noexcept {
            return data_ + size_;
        }

        constexpr std::reverse_iterator<const char *> rbegin() const noexcept {
            return std::reverse_iterator<const char *>(end());
        }
        constexpr std::reverse_iterator<const char *> crbegin() const noexcept {
            return rbegin();
        }
        constexpr std::reverse_iterator<const char *> rend() const noexcept {
            return std::reverse_iterator<const char *>(begin());
        }
        constexpr std::reverse_iterator<const char *> crend() const noexcept {
            return rend();
        }

        // ----- Capacity & Modifiers

        constexpr string &erase(std::size_t pos = 0, std::size_t count = npos) {
            if (pos > size_) {
                throw std::out_of_range("hp::string::erase: pos out of range");
            }

            const std::size_t rcount = std::min(count, size_ - pos);
            if (rcount == 0) {
                return *this;
            }

            std::char_traits<char>::move(data_ + pos, data_ + pos + rcount, size_ - pos - rcount);
            size_ -= rcount;
            data_[size_] = '\0';
            return *this;
        }

        constexpr char *erase(char *it) {
            const std::size_t pos = static_cast<std::size_t>(it - data_);
            erase(pos, 1);
            return data_ + pos;
        }

        constexpr std::size_t capacity() const noexcept {
            return capacity_;
        }

        constexpr string &insert(std::size_t pos, const char *str, std::size_t n) {
            if (pos > size_) {
                throw std::out_of_range("hp::string::insert: pos out of range");
            }
            if (n == 0) {
                return *this;
            }

            if (n > npos - size_) {
                throw std::length_error("hp::string::insert: size exceeds maximum");
            }

            const std::size_t new_size = size_ + n;

            ensure_capacity(new_size);
            std::char_traits<char>::move(data_ + pos + n, data_ + pos, size_ - pos);
            std::char_traits<char>::copy(data_ + pos, str, n);
            size_ = new_size;
            data_[size_] = '\0';
            return *this;
        }

        constexpr string &insert(std::size_t pos, std::size_t count, char ch) {
            if (pos > size_) {
                throw std::out_of_range("hp::string::insert: pos out of range");
            }

            if (count == 0)
                return *this;
            if (count > npos - size_) {
                throw std::length_error("hp::string::insert: size exceeds maximum");
            }

            const std::size_t new_size = size_ + count;

            ensure_capacity(new_size);
            std::char_traits<char>::move(data_ + pos + count, data_ + pos, size_ - pos);
            std::char_traits<char>::assign(data_ + pos, count, ch);
            size_ = new_size;
            data_[size_] = '\0';
            return *this;
        }

        constexpr string &insert(std::size_t pos, const string &str) {
            return insert(pos, str.data_, str.size_);
        }

        constexpr string &insert(std::size_t pos, const char *str) {
            return insert(pos, str, std::char_traits<char>::length(str));
        }

        constexpr string &insert(std::size_t pos, std::string_view sv) {
            return insert(pos, sv.data(), sv.size());
        }

        constexpr void clear() noexcept {
            size_ = 0;
            if (owned_) {
                data_[0] = '\0';
            }
        }

        constexpr void reserve(std::size_t new_cap) {
            if (new_cap <= capacity_) {
                return;
            }
            ensure_capacity(new_cap);
        }

        constexpr void resize(std::size_t count, char ch = '\0') {
            if (count > capacity_) {
                reserve(std::max(count, capacity_ * 2));
            }
            if (count > size_ && !owned_) {
                ensure_capacity(std::max(count, capacity_ * 2));
            }
            if (count > size_) {
                std::char_traits<char>::assign(data_ + size_, count - size_, ch);
            }
            size_ = count;
            if (owned_) {
                data_[size_] = '\0';
            }
        }

        // ----- Modifiers
        constexpr void remove_prefix(std::size_t n) noexcept {
            if (n == 0) {
                return;
            }
            if (n >= size_) {
                size_ = 0;
                if (owned_) {
                    data_[0] = '\0';
                }
                return;
            }
            std::char_traits<char>::move(data_, data_ + n, size_ - n);
            size_ -= n;
            data_[size_] = '\0';
        }

        constexpr void remove_suffix(std::size_t n) noexcept {
            if (n >= size_) {
                size_ = 0;
            } else {
                size_ -= n;
            }
            if (owned_) {
                data_[size_] = '\0';
            }
        }

        constexpr void swap(string &other) noexcept {
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
            std::swap(data_, other.data_);
            std::swap(owned_, other.owned_);
        }

        constexpr string substr(std::size_t pos = 0, std::size_t count = npos) const {
            if (pos > size_) {
                throw std::out_of_range("hp::string::substr: pos out of range");
            }

            const std::size_t rlen = std::min(count, size_ - pos);
            return string(data_ + pos, rlen);
        }

        // ----- Search & Prefix/Suffix Checks
        constexpr bool starts_with(const string &sv) const noexcept {
            return size_ >= sv.size_ && std::char_traits<char>::compare(data_, sv.data_, sv.size_) == 0;
        }

        constexpr bool starts_with(char ch) const noexcept {
            return !empty() && front() == ch;
        }

        constexpr bool ends_with(const string &sv) const noexcept {
            return size_ >= sv.size_ && std::char_traits<char>::compare(data_ + (size_ - sv.size_), sv.data_, sv.size_) == 0;
        }

        constexpr bool ends_with(char ch) const noexcept {
            return !empty() && back() == ch;
        }

        constexpr std::size_t find(const string &sv, std::size_t pos = 0) const noexcept {
            if (pos > size_ || sv.size_ > size_ - pos) {
                return npos;
            }

            if (sv.empty()) {
                return pos;
            }

            const auto it = std::search(begin() + pos, end(), sv.begin(), sv.end());
            return it == end() ? npos : static_cast<std::size_t>(it - begin());
        }

        constexpr std::size_t find(char ch, std::size_t pos = 0) const noexcept {
            if (pos >= size_) {
                return npos;
            }

            const auto *res = std::char_traits<char>::find(data_ + pos, size_ - pos, ch);
            return res ? static_cast<std::size_t>(res - data_) : npos;
        }

        constexpr std::size_t rfind(const string &sv, std::size_t pos = npos) const noexcept {
            if (sv.size_ > size_) {
                return npos;
            }

            if (sv.empty()) {
                return std::min(pos, size_);
            }

            const std::size_t last_start = std::min(pos, size_ - sv.size_);
            for (std::size_t i = last_start + 1; i > 0; --i) {
                const std::size_t idx = i - 1;
                if (std::char_traits<char>::compare(data_ + idx, sv.data_, sv.size_) == 0) {
                    return idx;
                }
            }
            return npos;
        }

        constexpr std::size_t rfind(char ch, std::size_t pos = npos) const noexcept {
            if (empty()) {
                return npos;
            }
            for (std::size_t i = std::min(pos, size_ - 1) + 1; i > 0; --i) {
                if (data_[i - 1] == ch) {
                    return i - 1;
                }
            }
            return npos;
        }

        constexpr bool contains(const string &sv) const noexcept {
            return find(sv) != npos;
        }

        constexpr bool contains(char ch) const noexcept {
            return find(ch) != npos;
        }

        // ----- Comparisons
        constexpr bool operator==(const string &other) const noexcept {
            return size_ == other.size_ && std::char_traits<char>::compare(data_, other.data_, size_) == 0;
        }

        constexpr auto operator<=>(const string &other) const noexcept {
            const int cmp = std::char_traits<char>::compare(data_, other.data_, std::min(size_, other.size_));

            if (cmp != 0)
                return cmp <=> 0;
            return size_ <=> other.size_;
        }

        // ----- Conversions & Stream Output
        constexpr explicit operator std::string_view() const noexcept {
            return std::string_view(data_, size_);
        }

        explicit operator std::string() const {
            return std::string(data_, size_);
        }

        friend std::ostream &operator<<(std::ostream &os, const string &s) {
            return os.write(s.data_, static_cast<std::streamsize>(s.size_));
        }
    };

} // namespace hp

// ----- std::format & std::print(ln) Support
template <>
struct std::formatter<hp::string> : std::formatter<std::string_view> {
    auto format(hp::string s, std::format_context &ctx) const {
        return std::formatter<std::string_view>::format(std::string_view(s), ctx);
    }
};
