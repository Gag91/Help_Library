#include "hp/type/vector.hpp"
#include "hp/print/print.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <forward_list>
#include <memory_resource>

static_assert(std::ranges::contiguous_range<hp::vector<int>>);
static_assert(std::ranges::contiguous_range<const hp::vector<int>>);
static_assert(std::ranges::sized_range<hp::vector<int>>);
static_assert(std::ranges::random_access_range<hp::vector<int>>);
static_assert(std::ranges::bidirectional_range<hp::vector<int>>);
static_assert(std::ranges::forward_range<hp::vector<int>>);
static_assert(std::ranges::input_range<hp::vector<int>>);
static_assert(std::ranges::output_range<hp::vector<int>, int>);

static_assert(sizeof(hp::vector<int>) == 3 * sizeof(void *));
static_assert(sizeof(hp::vector<int, std::pmr::polymorphic_allocator<int>>) == 4 * sizeof(void *));

static_assert(std::same_as<typename hp::vector<int>::value_type, int>);
static_assert(std::same_as<typename hp::vector<int>::iterator, int *>);
static_assert(std::same_as<typename hp::vector<int>::const_iterator, const int *>);

static_assert(requires(hp::vector<int> v) { v; });
static_assert(requires(hp::vector<int> v) { v[0u]; });
static_assert(requires(hp::vector<int> v) { v[std::uint8_t{0}]; });
static_assert(requires(hp::vector<int> v) { v[std::uint16_t{0}]; });
static_assert(requires(hp::vector<int> v) { v[std::uint32_t{0}]; });
static_assert(requires(hp::vector<int> v) { v[0uz]; });

constexpr bool test_constexpr_vector_core() {
    hp::vector<int> v;
    if (!v.empty() || v.size() != 0uz || v.capacity() != 0uz || v.data() != nullptr)
        return false;

    for (int i = 1; i <= 20; ++i) {
        auto &ref = v.push_back(i * 10);
        if (ref != i * 10)
            return false;
    }

    if (v.size() != 20uz || v.capacity() < 20uz || v.front() != 10 || v.back() != 200)
        return false;

    v.push_front(5);
    v.push_front(1);
    if (v.size() != 22uz || v.front() != 1 || v[1u] != 5 || v[2u] != 10)
        return false;

    v.insert(std::uint8_t{0}, 0);
    v.insert(std::uint16_t{4}, 15);
    v.insert(v.size(), 210);
    if (v.size() != 25uz || v.front() != 0 || v[4u] != 15 || v.back() != 210)
        return false;

    auto it_first = v.erase(std::uint8_t{0});
    if (*it_first != 1 || v.size() != 24uz)
        return false;

    auto it_mid = v.erase(std::uint16_t{3});
    if (*it_mid != 20 || v.size() != 23uz)
        return false;

    auto it_last = v.erase(22uz);
    if (it_last != v.end() || v.size() != 22uz || v.back() != 200)
        return false;

    const auto cap_before_clear = v.capacity();
    v.clear();
    if (!v.empty() || v.size() != 0uz || v.capacity() != cap_before_clear)
        return false;

    v.reserve(64u);
    if (v.capacity() < 64uz || !v.empty())
        return false;

    return true;
}

constexpr bool test_constexpr_vector_ranges_and_copy_move() {
    hp::vector<int> a = {9, 4, 7, 1, 8, 2, 6, 3, 5};
    std::ranges::sort(a);
    for (int i = 0; i < 9; ++i) {
        if (a[static_cast<unsigned>(i)] != i + 1)
            return false;
    }

    hp::vector<int> b = a;
    if (!(a == b) || (a != b))
        return false;

    b[8u] = 99;
    if (!(a < b) || !(b > a) || (a == b))
        return false;

    hp::vector<int> c = std::move(b);
    if (c.size() != 9uz || c[8u] != 99 || !b.empty())
        return false;

    a = c;
    if (!(a == c) || a[8u] != 99)
        return false;

    hp::vector<int> d;
    d = std::move(a);
    if (d.size() != 9uz || d[8u] != 99 || !a.empty())
        return false;

    auto pipeline = std::views::iota(1, 30) | std::views::filter([](int n) { return n % 2 == 1; }) | std::views::transform([](int n) { return n * 3; }) | std::views::take(5);

    hp::vector<int> from_view(std::from_range, pipeline);
    if (from_view.size() != 5uz || from_view[0u] != 3 || from_view[4u] != 27)
        return false;

    from_view.append_range(std::views::iota(100, 103));
    if (from_view.size() != 8uz || from_view[5u] != 100 || from_view[7u] != 102)
        return false;

    return true;
}

static_assert(test_constexpr_vector_core());
static_assert(test_constexpr_vector_ranges_and_copy_move());

template <typename T>
struct TrackingAllocator {
    using value_type = T;

    static inline std::size_t live_bytes = 0;
    static inline std::size_t alloc_count = 0;
    static inline std::size_t dealloc_count = 0;

    TrackingAllocator() noexcept = default;
    template <typename U>
    TrackingAllocator(const TrackingAllocator<U> &) noexcept {}

    [[nodiscard]] T *allocate(std::size_t n) {
        ++alloc_count;
        live_bytes += n * sizeof(T);
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T *p, std::size_t n) noexcept {
        ++dealloc_count;
        live_bytes -= n * sizeof(T);
        std::allocator<T>{}.deallocate(p, n);
    }

    bool operator==(const TrackingAllocator &) const noexcept = default;

    static void reset() noexcept {
        live_bytes = 0;
        alloc_count = 0;
        dealloc_count = 0;
    }
};

struct NoDefault {
    int value;
    explicit NoDefault(int v) noexcept : value(v) {}
    bool operator==(const NoDefault &) const = default;
};

struct Tracked {
    static inline int live_objects = 0;
    static inline int copies = 0;
    static inline int moves = 0;

    int id;

    explicit Tracked(int v) noexcept : id(v) {
        ++live_objects;
    }
    Tracked(const Tracked &o) noexcept : id(o.id) {
        ++live_objects;
        ++copies;
    }
    Tracked(Tracked &&o) noexcept : id(o.id) {
        o.id = -1;
        ++live_objects;
        ++moves;
    }
    Tracked &operator=(const Tracked &o) noexcept {
        id = o.id;
        ++copies;
        return *this;
    }
    Tracked &operator=(Tracked &&o) noexcept {
        id = o.id;
        o.id = -1;
        ++moves;
        return *this;
    }
    ~Tracked() {
        --live_objects;
    }

    static void reset() noexcept {
        copies = 0;
        moves = 0;
    }
};

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

void test_allocator_accounting_and_rule_of_five() {
    TrackingAllocator<Tracked>::reset();
    assert(Tracked::live_objects == 0);
    assert(TrackingAllocator<Tracked>::live_bytes == 0);

    {
        hp::vector<Tracked, TrackingAllocator<Tracked>> v;
        assert(TrackingAllocator<Tracked>::alloc_count == 0);

        for (int i = 0; i < 10; ++i) {
            v.emplace_back(i * 10);
        }

        assert(v.size() == 10uz);
        assert(Tracked::live_objects == 10);
        assert(TrackingAllocator<Tracked>::live_bytes == v.capacity() * sizeof(Tracked));

        v.erase(0u);
        assert(v.size() == 9uz);
        assert(Tracked::live_objects == 9);
        assert(v.front().id == 10);

        auto copy_v = v;
        assert(copy_v.size() == 9uz);
        assert(Tracked::live_objects == 18);

        hp::vector<Tracked, TrackingAllocator<Tracked>> assigned_v;
        assigned_v.emplace_back(999);
        assigned_v = copy_v;
        assert(assigned_v.size() == 9uz);
        assert(Tracked::live_objects == 27);

        auto moved_v = std::move(assigned_v);
        assert(moved_v.size() == 9uz);
        assert(assigned_v.empty());
        assert(Tracked::live_objects == 27);

        hp::vector<Tracked, TrackingAllocator<Tracked>> move_assigned_v;
        move_assigned_v.emplace_back(888);
        move_assigned_v = std::move(moved_v);
        assert(move_assigned_v.size() == 9uz);
        assert(Tracked::live_objects == 27);

        v.clear();
        assert(v.empty());
        assert(Tracked::live_objects == 18);
    }

    assert(Tracked::live_objects == 0);
    assert(TrackingAllocator<Tracked>::live_bytes == 0);
    assert(TrackingAllocator<Tracked>::alloc_count == TrackingAllocator<Tracked>::dealloc_count);
}

void test_pmr_stack_arena() {
    std::array<std::byte, 1024> stack_buf{};
    std::pmr::monotonic_buffer_resource arena(
        stack_buf.data(),
        stack_buf.size(),
        std::pmr::null_memory_resource());

    hp::vector<int, std::pmr::polymorphic_allocator<int>> v(&arena);
    for (int i = 0; i < 50; ++i) {
        v.push_back(i * 2);
    }

    assert(v.size() == 50uz);
    assert(v.front() == 0);
    assert(v.back() == 98);

    v.insert(25u, 7777);
    assert(v.size() == 51uz);
    assert(v[25u] == 7777);

    v.erase(25u);
    assert(v.size() == 50uz);
    assert(v[25u] == 50);
}

void test_non_default_constructible_and_move_only() {
    hp::vector<NoDefault> nd;
    nd.emplace_back(10);
    nd.push_back(NoDefault(20));
    nd.push_front(NoDefault(5));
    nd.insert(2u, NoDefault(15));
    assert(nd.size() == 4uz);
    assert(nd[0u].value == 5);
    assert(nd[1u].value == 10);
    assert(nd[2u].value == 15);
    assert(nd[3u].value == 20);

    nd.erase(1u);
    assert(nd.size() == 3uz);
    assert(nd[1u].value == 15);

    hp::vector<std::unique_ptr<std::string>> ptrs;
    ptrs.push_back(std::make_unique<std::string>("first"));
    ptrs.push_back(std::make_unique<std::string>("third"));
    ptrs.push_front(std::make_unique<std::string>("zero"));
    ptrs.insert(2u, std::make_unique<std::string>("second"));
    ptrs.emplace_back(new std::string("fourth"));

    assert(ptrs.size() == 5uz);
    assert(*ptrs[0u] == "zero");
    assert(*ptrs[1u] == "first");
    assert(*ptrs[2u] == "second");
    assert(*ptrs[3u] == "third");
    assert(*ptrs[4u] == "fourth");
    ptrs.erase(2u);
    assert(ptrs.size() == 4uz);
    assert(*ptrs[2u] == "third");
    ptrs.clear();
    assert(ptrs.empty());
}
void test_ranges_span_and_exceptions() {
    std::forward_list flist = {10, 20, 30, 40};
    hp::vector v(flist);
    assert(v.size() == 4uz);
    std::span s(v);
    assert(s.size() == 4uz);
    s[0] = 99;
    assert(v[0u] == 99);
    std::vector rev(v.rbegin(), v.rend());
    assert((rev == std::vector{40, 30, 20, 99}));
    const auto &cv = v;
    expect_specific_throw<std::out_of_range>([&] { (void)v.at(4u); });
    expect_specific_throw<std::out_of_range>([&] { (void)cv.at(100uz); });
    expect_specific_throw<std::out_of_range>([&] { v.erase(4u); });
    expect_specific_throw<std::out_of_range>([&] { v.insert(5u, 123); });
    v.clear();
}

int main() {
    test_allocator_accounting_and_rule_of_five();
    test_pmr_stack_arena();
    test_non_default_constructible_and_move_only();
    test_ranges_span_and_exceptions();

    const hp::vector<char> msg = {'A', 'l', 'l', ' ', 't', 'e', 's', 't', ' ', 'p', 'a', 's', 's', 'e', 'd'};
    for (char c : msg)
        hp::print("{}", c);
}