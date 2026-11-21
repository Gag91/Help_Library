#include "hp/type/array.hpp"
#include "hp/print/print.hpp"

#include <cassert>
#include <forward_list>

static_assert(std::ranges::contiguous_range<hp::array<int, 4u>>);
static_assert(std::ranges::contiguous_range<const hp::array<int, 4u>>);
static_assert(std::ranges::sized_range<hp::array<int, 4u>>);
static_assert(std::ranges::random_access_range<hp::array<int, 4u>>);
static_assert(std::ranges::bidirectional_range<hp::array<int, 4u>>);
static_assert(std::ranges::forward_range<hp::array<int, 4u>>);
static_assert(std::ranges::input_range<hp::array<int, 4u>>);
static_assert(std::ranges::output_range<hp::array<int, 4u>, int>);

static_assert(std::is_trivially_copyable_v<hp::array<int, 8u>>);
static_assert(std::is_trivially_destructible_v<hp::array<int, 8u>>);
static_assert(std::is_nothrow_default_constructible_v<hp::array<int, 8u>>);
static_assert(std::is_nothrow_move_constructible_v<hp::array<int, 8u>>);
static_assert(std::is_nothrow_move_assignable_v<hp::array<int, 8u>>);
static_assert(!std::is_trivially_destructible_v<hp::array<std::string, 8u>>);

static_assert(sizeof(hp::array<std::uint8_t, std::uint8_t{1}>) == 2);
static_assert(sizeof(hp::array<std::uint8_t, std::uint8_t{3}>) == 4);
static_assert(sizeof(hp::array<std::uint8_t, std::uint8_t{7}>) == 8);
static_assert(sizeof(hp::array<std::uint16_t, std::uint16_t{3}>) == 8);
static_assert(sizeof(hp::array<std::uint32_t, std::uint32_t{3}>) == 16);

static_assert(std::same_as<typename hp::array<int, std::uint8_t{5}>::size_type, std::uint8_t>);
static_assert(std::same_as<typename hp::array<int, std::uint16_t{5}>::size_type, std::uint16_t>);
static_assert(std::same_as<typename hp::array<int, std::uint32_t{5}>::size_type, std::uint32_t>);
static_assert(std::same_as<typename hp::array<int, 5uz>::size_type, std::size_t>);
static_assert(std::same_as<typename hp::array<int, 5u>::value_type, int>);
static_assert(std::same_as<typename hp::array<int, 5u>::iterator, int *>);
static_assert(std::same_as<typename hp::array<int, 5u>::const_iterator, const int *>);

static_assert(requires(hp::array<int, 4u> a) { a[0u]; });
static_assert(requires(hp::array<int, 4u> a) { a[std::uint8_t{0}]; });
static_assert(requires(hp::array<int, 4u> a) { a[std::uint16_t{0}]; });
static_assert(requires(hp::array<int, 4u> a) { a[std::uint32_t{0}]; });
static_assert(requires(hp::array<int, 4u> a) { a[0uz]; });

constexpr bool test_constexpr_core_and_algorithms() {
    hp::array<int, 12u> arr;
    if (!arr.empty() || arr.size() != 0u || arr.capacity() != 12u)
        return false;

    if (arr.size<std::uint8_t>() != std::uint8_t{0})
        return false;
    if (arr.capacity<std::uint16_t>() != std::uint16_t{12})
        return false;

    for (int i = 1; i <= 5; ++i) {
        auto &ref = arr.push_back(i * 10);
        if (ref != i * 10)
            return false;
    }

    if (arr.size() != 5u || arr.empty() || arr.front() != 10 || arr.back() != 50)
        return false;

    arr.push_front(5);
    arr.push_front(1);
    if (arr.size() != 7u || arr.front() != 1 || arr[1u] != 5 || arr[2u] != 10)
        return false;

    auto &ins1 = arr.insert(std::uint8_t{0}, 0);
    auto &ins2 = arr.insert(std::uint16_t{4}, 15);
    auto &ins3 = arr.insert(arr.size(), 60);
    if (ins1 != 0 || ins2 != 15 || ins3 != 60 || arr.size() != 10u)
        return false;

    const int expected_after_inserts[] = {0, 1, 5, 10, 15, 20, 30, 40, 50, 60};
    for (std::size_t i = 0; i < 10; ++i) {
        if (arr[i] != expected_after_inserts[i] || arr.at(i) != expected_after_inserts[i])
            return false;
    }

    const auto &carr = arr;
    if (carr.front() != 0 || carr.back() != 60 || carr.data()[3] != 10 || carr.at(5u) != 20)
        return false;

    auto it_first = arr.erase(std::uint8_t{0});
    if (*it_first != 1 || arr.size() != 9u)
        return false;

    auto it_mid = arr.erase(std::uint16_t{3});
    if (*it_mid != 20 || arr.size() != 8u)
        return false;

    auto it_last = arr.erase(7uz);
    if (it_last != arr.end() || arr.size() != 7u || arr.back() != 50)
        return false;

    arr.resize(std::uint8_t{11}, 77);
    if (arr.size() != 11u || arr[7u] != 77 || arr[10u] != 77)
        return false;

    arr.resize(std::uint16_t{4});
    if (arr.size() != 4u || arr.back() != 20)
        return false;

    int forward_sum = 0;
    for (auto it = arr.cbegin(); it != arr.cend(); ++it)
        forward_sum += *it;
    if (forward_sum != 36)
        return false;

    int reverse_check[] = {20, 10, 5, 1};
    std::size_t r_idx = 0;
    for (auto rit = arr.crbegin(); rit != arr.crend(); ++rit) {
        if (*rit != reverse_check[r_idx++])
            return false;
    }

    arr.clear();
    if (!arr.empty() || arr.size() != 0u || arr.begin() != arr.end())
        return false;

    return true;
}

constexpr bool test_constexpr_ranges_and_comparisons() {
    hp::array<int, 8u> a = {8, 3, 6, 1, 7, 2, 5, 4};
    std::ranges::sort(a);
    for (int i = 0; i < 8; ++i) {
        if (a[static_cast<unsigned>(i)] != i + 1)
            return false;
    }

    std::ranges::reverse(a);
    if (a.front() != 8 || a.back() != 1)
        return false;

    auto pipeline = std::views::iota(1, 20) | std::views::filter([](int n) { return n % 3 == 0; }) | std::views::transform([](int n) { return n * 2; }) | std::views::take(4);

    hp::array<int, 6u> b(std::from_range, pipeline);
    if (b.size() != 4u || b[0u] != 6 || b[1u] != 12 || b[2u] != 18 || b[3u] != 24)
        return false;

    auto extra = std::views::iota(100, 102);
    b.append_range(extra);
    if (b.size() != 6u || b[4u] != 100 || b[5u] != 101)
        return false;

    hp::array<int, 6u> c = b;
    if (!(b == c) || (b != c) || (b < c) || (b > c) || !(b <= c) || !(b >= c))
        return false;

    c[5u] = 200;
    if (!(b < c) || !(b <= c) || !(c > b) || !(c >= b) || (b == c) || !(b != c))
        return false;

    hp::array<int, 6u> shorter = {6, 12, 18};
    if (!(shorter < b) || !(b > shorter))
        return false;

    return true;
}

static_assert(test_constexpr_core_and_algorithms());
static_assert(test_constexpr_ranges_and_comparisons());

struct Tracked {
    static inline int live_objects = 0;
    static inline int copy_constructions = 0;
    static inline int move_constructions = 0;
    static inline int copy_assignments = 0;
    static inline int move_assignments = 0;

    int id = 0;
    bool moved_from = false;

    Tracked() noexcept : id(0) {
        ++live_objects;
    }
    explicit Tracked(int v) noexcept : id(v) {
        ++live_objects;
    }

    Tracked(const Tracked &other) noexcept : id(other.id) {
        ++live_objects;
        ++copy_constructions;
    }

    Tracked(Tracked &&other) noexcept : id(other.id) {
        other.moved_from = true;
        ++live_objects;
        ++move_constructions;
    }

    Tracked &operator=(const Tracked &other) noexcept {
        id = other.id;
        moved_from = false;
        ++copy_assignments;
        return *this;
    }

    Tracked &operator=(Tracked &&other) noexcept {
        id = other.id;
        moved_from = false;
        other.moved_from = true;
        ++move_assignments;
        return *this;
    }

    ~Tracked() {
        --live_objects;
    }

    static void reset_counters() noexcept {
        copy_constructions = 0;
        move_constructions = 0;
        copy_assignments = 0;
        move_assignments = 0;
    }
};

template <typename Fn>
void expect_throw(Fn &&fn) {
    bool threw = false;
    try {
        fn();
    } catch (const std::exception &) {
        threw = true;
    }
    assert(threw);
}

template <typename ExceptionType, typename Fn>
void expect_specific_throw(Fn &&fn) {
    bool threw = false;
    try {
        fn();
    } catch (const ExceptionType &) {
        threw = true;
    } catch (...) {
        assert(false);
    }
    assert(threw);
}

void test_lifecycle_and_move_semantics() {
    assert(Tracked::live_objects == 0);
    {
        hp::array<Tracked, 5u> arr;
        assert(Tracked::live_objects == 5);

        Tracked::reset_counters();
        arr.emplace_back(100);
        arr.push_back(Tracked(200));
        arr.push_front(Tracked(50));
        arr.insert(2u, Tracked(150));

        assert(Tracked::copy_constructions == 0);
        assert(Tracked::copy_assignments == 0);
        assert(arr.size() == 4u);
        assert(arr[0u].id == 50);
        assert(arr[1u].id == 100);
        assert(arr[2u].id == 150);
        assert(arr[3u].id == 200);

        Tracked lvalue_item(300);
        arr.push_back(lvalue_item);
        assert(Tracked::copy_assignments == 1);
        assert(arr.back().id == 300);

        arr.erase(1u);
        assert(arr.size() == 4u);
        assert(arr[0u].id == 50);
        assert(arr[1u].id == 150);
        assert(arr[2u].id == 200);
        assert(arr[3u].id == 300);

        arr.resize(2u);
        assert(arr.size() == 2u);
        assert(arr[0u].id == 50);
        assert(arr[1u].id == 150);

        arr.clear();
        assert(arr.empty());
    }
    assert(Tracked::live_objects == 0);
}

void test_move_only_types() {
    hp::array<std::unique_ptr<std::string>, 6u> ptrs;
    ptrs.push_back(std::make_unique<std::string>("alpha"));
    ptrs.push_back(std::make_unique<std::string>("gamma"));
    ptrs.push_front(std::make_unique<std::string>("zero"));
    ptrs.insert(2u, std::make_unique<std::string>("beta"));
    ptrs.emplace_back(new std::string("delta"));
    assert(ptrs.size() == 5u);
    assert(*ptrs[0u] == "zero");
    assert(*ptrs[1u] == "alpha");
    assert(*ptrs[2u] == "beta");
    assert(*ptrs[3u] == "gamma");
    assert(*ptrs[4u] == "delta");
    auto moved_array = std::move(ptrs);
    assert(moved_array.size() == 5u);
    assert(*moved_array[2u] == "beta");
    moved_array.erase(0u);
    assert(moved_array.size() == 4u);
    assert(*moved_array.front() == "alpha");
    moved_array.resize(2u);
    assert(moved_array.size() == 2u);
    assert(*moved_array[0u] == "alpha");
    assert(*moved_array[1u] == "beta");
    moved_array.clear();
    assert(moved_array.empty());
}
void test_ranges_and_span_interoperability() {
    std::vector vec = {10, 20, 30, 40, 50};
    hp::array<int, 8u> from_vec(std::from_range, vec);
    assert(from_vec.size() == 5u);
    assert(std::ranges::equal(from_vec, vec));
    std::forward_list flist = {60, 70, 80};
    from_vec.append_range(flist);
    assert(from_vec.size() == 8u);
    assert(from_vec[5u] == 60);
    assert(from_vec[6u] == 70);
    assert(from_vec[7u] == 80);
    std::span full_span(from_vec);
    assert(full_span.size() == 8u);
    full_span[0] = 999;
    assert(from_vec[0u] == 999);
    const auto &const_arr = from_vec;
    std::span const_span(const_arr);
    assert(const_span.size() == 8u);
    assert(const_span[0] == 999);
    auto doubled = const_arr | std::views::take(3) | std::views::transform([](int v) { return v * 2; });
    hp::array<int, 4u> small_arr(std::from_range, doubled);
    assert(small_arr.size() == 3u);
    assert(small_arr[0u] == 1998);
    assert(small_arr[1u] == 40);
    assert(small_arr[2u] == 60);
}
void test_all_exception_paths() {
    hp::array<std::string, 3u> arr;

    arr.push_back("one");
    arr.push_back("two");
    const auto &carr = arr;
    arr.push_back("three");
    assert(arr.size() == 3u);
}
void test_narrow_capacity_boundary() {
    hp::array<int, std::uint8_t{255}> max_u8_arr;
    assert(max_u8_arr.capacity() == std::uint8_t{255});
    for (int i = 0; i < 255; ++i)
        max_u8_arr.push_back(i);
    assert(max_u8_arr.size() == std::uint8_t{255});
    assert(max_u8_arr.front() == 0);
    assert(max_u8_arr.back() == 254);
}

int main() {
    test_lifecycle_and_move_semantics();
    test_move_only_types();
    test_ranges_and_span_interoperability();
    test_all_exception_paths();
    test_narrow_capacity_boundary();

    constexpr hp::array<char, 15> msg = {'A', 'l', 'l', ' ', 't', 'e', 's', 't', ' ', 'p', 'a', 's', 's', 'e', 'd'};
    hp::print("{}", msg);
}