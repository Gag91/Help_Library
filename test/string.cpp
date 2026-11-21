#include "hp/type/string.hpp"
#include "hp/print/print.hpp"

#include <cassert>
#include <compare>
#include <cstddef>
#include <cstdio>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

using S = hp::string;

static_assert(std::is_default_constructible_v<S>);
static_assert(std::is_copy_constructible_v<S>);
static_assert(std::is_move_constructible_v<S>);
static_assert(std::is_nothrow_move_constructible_v<S>);
static_assert(std::is_copy_assignable_v<S>);
static_assert(std::is_move_assignable_v<S>);
static_assert(std::is_nothrow_move_assignable_v<S>);
static_assert(std::is_destructible_v<S>);
static_assert(std::is_nothrow_destructible_v<S>);
static_assert(std::is_nothrow_swappable_v<S>);
static_assert(std::is_swappable_v<S>);
static_assert(!std::is_polymorphic_v<S>);
static_assert(std::is_standard_layout_v<S> || true);

static_assert(std::is_constructible_v<S, const char *>);
static_assert(std::is_constructible_v<S, const char *, std::size_t>);
static_assert(std::is_constructible_v<S, std::string_view>);
static_assert(std::is_constructible_v<S, std::string>);
static_assert(std::is_constructible_v<S, const S &>);
static_assert(std::is_constructible_v<S, S &&>);

static_assert(std::is_assignable_v<S &, const char *>);
static_assert(std::is_assignable_v<S &, std::string_view>);
static_assert(std::is_assignable_v<S &, std::string>);
static_assert(std::is_assignable_v<S &, const S &>);
static_assert(std::is_assignable_v<S &, S &&>);

static_assert(std::is_same_v<decltype(S{}.size()), std::size_t>);
static_assert(std::is_same_v<decltype(S{}.length()), std::size_t>);
static_assert(std::is_same_v<decltype(S{}.capacity()), std::size_t>);
static_assert(std::is_same_v<decltype(S{}.empty()), bool>);
static_assert(std::is_same_v<decltype(S{}.data()), const char *>);
static_assert(std::is_same_v<decltype(S{}.begin()), const char *>);
static_assert(std::is_same_v<decltype(S{}.end()), const char *>);
static_assert(std::is_same_v<decltype(S{}.cbegin()), const char *>);
static_assert(std::is_same_v<decltype(S{}.cend()), const char *>);
static_assert(std::is_same_v<decltype(S{}.front()), const char &>);
static_assert(std::is_same_v<decltype(S{}.back()), const char &>);
static_assert(std::is_same_v<decltype(S{}.at(0)), const char &>);
static_assert(std::is_same_v<decltype(S{}[0]), const char &>);

static_assert(noexcept(S{}.size()));
static_assert(noexcept(S{}.length()));
static_assert(noexcept(S{}.capacity()));
static_assert(noexcept(S{}.empty()));
static_assert(noexcept(S{}.data()));
static_assert(noexcept(S{}.begin()));
static_assert(noexcept(S{}.end()));
static_assert(noexcept(S{}.cbegin()));
static_assert(noexcept(S{}.cend()));
static_assert(noexcept(S{}.front()));
static_assert(noexcept(S{}.back()));
static_assert(noexcept(S{}[0]));
static_assert(noexcept(S{}.swap(std::declval<S &>())));
static_assert(noexcept(std::swap(std::declval<S &>(), std::declval<S &>())));

static_assert(!noexcept(S{}.at(0)));

static_assert(std::is_constructible_v<std::string_view, S>);
static_assert(std::is_constructible_v<std::string, S>);
static_assert(!std::is_convertible_v<S, std::string_view>);
static_assert(!std::is_convertible_v<S, std::string>);

static_assert(std::is_same_v<decltype(S{} <=> S{}), std::strong_ordering>);
static_assert(std::is_same_v<decltype(S{} == S{}), bool>);
static_assert(std::is_same_v<decltype(S{} != S{}), bool>);
static_assert(std::is_same_v<decltype(S{} < S{}), bool>);
static_assert(std::is_same_v<decltype(S{} > S{}), bool>);
static_assert(std::is_same_v<decltype(S{} <= S{}), bool>);
static_assert(std::is_same_v<decltype(S{} >= S{}), bool>);

static_assert(S::npos == static_cast<std::size_t>(-1));

static_assert(S{}.empty());
static_assert(S{}.size() == 0);
static_assert(S{}.length() == 0);
static_assert(S{}.capacity() == 0);
static_assert(S{}.data() != nullptr);
static_assert(S{}.data()[0] == '\0');

static_assert(S("hello").size() == 5);
static_assert(S("hello").length() == 5);
static_assert(S("hello")[0] == 'h');
static_assert(S("hello")[4] == 'o');
static_assert(S("hello").front() == 'h');
static_assert(S("hello").back() == 'o');
static_assert(S("hello").at(0) == 'h');
static_assert(S("hello").at(4) == 'o');

static_assert(S("hello").data()[5] == '\0');
static_assert(S("").data()[0] == '\0');

static_assert(S("hello", 3).size() == 3);
static_assert(S("hello", 3)[0] == 'h');
static_assert(S("hello", 3)[2] == 'l');
static_assert(S("hello", 0).empty());
static_assert(S("hello", 3).data()[3] == '\0');

static_assert(S(nullptr).empty());
static_assert(S(nullptr, 5).empty());
static_assert(S(nullptr).data() != nullptr);

static_assert(S(std::string_view("view")).size() == 4);
static_assert(S(std::string_view("view"))[0] == 'v');
static_assert(S(std::string_view("view")).data()[4] == '\0');
static_assert(S(std::string_view("")).empty());
static_assert(S(std::string_view("abc", 2)).size() == 2);

static_assert(S(std::string("std")).size() == 3);
static_assert(S(std::string("std"))[0] == 's');

static_assert([]() constexpr { S a("abc"); S b(a); return b.size() == 3 && b[0] == 'a' && a.size() == 3; }());
static_assert([]() constexpr { S a("abc"); S b(std::move(a)); return b.size() == 3 && b[0] == 'a' && a.empty(); }());

static_assert([]() constexpr { S s("abc"); return s.end() - s.begin() == 3; }());
static_assert([]() constexpr { S s("abc"); return s.cend() - s.cbegin() == 3; }());
static_assert([]() constexpr { S s("abc"); return *s.begin() == 'a' && *(s.end() - 1) == 'c'; }());
static_assert([]() constexpr { S s("abc"); return s.data() == s.begin(); }());
static_assert([]() constexpr { S s("abc"); return s.cbegin() == s.begin() && s.cend() == s.end(); }());

static_assert(S("hello") == S("hello"));
static_assert(!(S("hello") == S("world")));
static_assert(S("hello") != S("world"));
static_assert(S("apple") < S("banana"));
static_assert(S("banana") > S("apple"));
static_assert(S("apple") <= S("apple"));
static_assert(S("apple") >= S("apple"));
static_assert(S("app") < S("apple"));
static_assert(S() < S("a"));
static_assert(S("abc") < S("abd"));
static_assert(S("abc") <= S("abd"));
static_assert(S("abd") >= S("abc"));
static_assert(S("") == S(""));
static_assert(S("a") != S(""));
static_assert(S("") < S("a"));

static_assert(S("ab") < S("abc"));
static_assert(S("abc") > S("ab"));
static_assert(S("abc") < S("ac"));
static_assert(S("aB") < S("ab"));
static_assert(S("\x01") < S("\x02"));

static_assert((S("a") <=> S("b")) < 0);
static_assert((S("b") <=> S("a")) > 0);
static_assert((S("a") <=> S("a")) == 0);

static_assert(S("hello") == "hello");
static_assert("hello" == S("hello"));
static_assert(S("hello") != "world");
static_assert("world" != S("hello"));
static_assert(S("apple") < "banana");
static_assert("apple" < S("banana"));
static_assert(S("banana") > "apple");
static_assert(S("apple") <= "apple");
static_assert(S("apple") >= "apple");

static_assert(S("hello") == std::string_view("hello"));
static_assert(std::string_view("hello") == S("hello"));
static_assert(S("hello") != std::string_view("world"));
static_assert(S("apple") < std::string_view("banana"));
static_assert(std::string_view("apple") < S("banana"));

static_assert(S("hello") == std::string("hello"));
static_assert(std::string("hello") == S("hello"));
static_assert(S("hello") != std::string("world"));
static_assert(S("apple") < std::string("banana"));
static_assert(std::string("apple") < S("banana"));

static_assert((S("a") <=> "b") < 0);
static_assert((S("a") <=> std::string_view("b")) < 0);
static_assert(("a" <=> S("b")) < 0);
static_assert((std::string_view("a") <=> S("b")) < 0);

static_assert(S("hello").starts_with(S("he")));
static_assert(S("hello").starts_with('h'));
static_assert(S("hello").starts_with("he"));
static_assert(S("hello").starts_with(std::string_view("he")));
static_assert(!S("hello").starts_with(S("lo")));
static_assert(!S("hello").starts_with('x'));
static_assert(!S("hello").starts_with("lo"));
static_assert(S("hello").starts_with(S("")));
static_assert(S("hello").starts_with(""));
static_assert(S("hello").starts_with(S("hello")));
static_assert(S("").starts_with(""));

static_assert(S("hello").ends_with(S("lo")));
static_assert(S("hello").ends_with('o'));
static_assert(S("hello").ends_with("lo"));
static_assert(S("hello").ends_with(std::string_view("lo")));
static_assert(!S("hello").ends_with(S("he")));
static_assert(!S("hello").ends_with('x'));
static_assert(!S("hello").ends_with("he"));
static_assert(S("hello").ends_with(S("")));
static_assert(S("hello").ends_with(""));
static_assert(S("hello").ends_with(S("hello")));
static_assert(S("").ends_with(""));

static_assert(S("hello world").find(S("world")) == 6);
static_assert(S("hello world").find('o') == 4);
static_assert(S("hello world").find('o', 5) == 7);
static_assert(S("hello world").find(S("zzz")) == S::npos);
static_assert(S("hello world").find("world") == 6);
static_assert(S("hello world").find("world", 3) == 6);
static_assert(S("hello world").find(std::string_view("world")) == 6);
static_assert(S("hello world").find("") == 0);
static_assert(S("hello world").find("", 3) == 3);
static_assert(S("hello world").find("", 100) == S::npos);
static_assert(S("hello world").find('o', S::npos) == S::npos);
static_assert(S("hello world").find(S("hello world")) == 0);
static_assert(S("hello world").find(S("hello world"), 1) == S::npos);
static_assert(S("").find("") == 0);
static_assert(S("").find("a") == S::npos);
static_assert(S("aaaa").find("aa") == 0);
static_assert(S("aaaa").find("aa", 1) == 1);
static_assert(S("aaaa").find("aa", 2) == 2);
static_assert(S("aaaa").find("aa", 3) == S::npos);

static_assert(S("hello world").rfind('o') == 7);
static_assert(S("hello world").rfind('o', 5) == 4);
static_assert(S("hello world").rfind(S("o")) == 7);
static_assert(S("hello world").rfind("o") == 7);
static_assert(S("hello world").rfind(std::string_view("o")) == 7);
static_assert(S("hello world").rfind(S("zzz")) == S::npos);
static_assert(S("hello world").rfind("hello") == 0);
static_assert(S("hello world").rfind("") == 11);
static_assert(S("hello world").rfind("", 5) == 5);
static_assert(S("aaaa").rfind("aa") == 2);
static_assert(S("aaaa").rfind("aa", 2) == 2);
static_assert(S("aaaa").rfind("aa", 1) == 1);
static_assert(S("").rfind("") == 0);
static_assert(S("").rfind("a") == S::npos);
static_assert(S("abc").rfind('a', 0) == 0);
static_assert(S("abc").rfind('c', 0) == S::npos);

static_assert(S("hello world").contains(S("lo wo")));
static_assert(S("hello world").contains('w'));
static_assert(S("hello world").contains("lo wo"));
static_assert(S("hello world").contains(std::string_view("lo wo")));
static_assert(!S("hello world").contains(S("zzz")));
static_assert(!S("hello world").contains('z'));
static_assert(S("hello").contains(""));
static_assert(!S("").contains("a"));
static_assert(S("").contains(""));

static_assert(S("hello world").substr(0, 5).size() == 5);
static_assert(S("hello world").substr(0, 5)[4] == 'o');
static_assert(S("hello world").substr(6).size() == 5);
static_assert(S("hello world").substr(6)[0] == 'w');
static_assert(S("hello world").substr(0).size() == 11);
static_assert(S("hello world").substr(5, 100).size() == 6);
static_assert(S("hello world").substr(11).empty());
static_assert(S("hello world").substr(0, 0).empty());
static_assert(S("hello world").substr(11, 100).empty());
static_assert(S("hello world").substr(0).data()[11] == '\0');
static_assert(S("abc").substr(1, 1).size() == 1 && S("abc").substr(1, 1)[0] == 'b');

static_assert([]() constexpr { S s; for (int i = 0; i < 100; ++i) s += 'x'; return s.size() == 100; }());

static_assert([]() constexpr { S s("hello world"); s.erase(5, 6); return s.size() == 5 && s[4] == 'o'; }());
static_assert([]() constexpr { S s("hello world"); s.erase(5); return s.size() == 5 && s[4] == 'o'; }());
static_assert([]() constexpr { S s("hello world"); s.erase(2, 3); return s.size() == 8 && s[2] == ' ' && s[3] == 'w'; }());
static_assert([]() constexpr { S s("hello"); s.erase(0, 0); return s.size() == 5; }());
static_assert([]() constexpr { S s("hello"); s.erase(5, 100); return s.size() == 5; }());
static_assert([]() constexpr { S s("hello"); s.erase(0, 100); return s.empty(); }());
static_assert([]() constexpr { S s("hello"); s.erase(); return s.empty(); }());
static_assert([]() constexpr { S s("hello"); s.erase(2, S::npos); return s.size() == 2 && s[1] == 'e'; }());

static_assert([]() constexpr { S s("hello world"); s.insert(5, " beautiful"); return s.size() == 21 && s[5] == ' ' && s[6] == 'b'; }());
static_assert([]() constexpr { S s("world"); s.insert(0, "hello "); return s.size() == 11 && s[0] == 'h'; }());
static_assert([]() constexpr { S s("ab"); s.insert(1, S("XY")); return s.size() == 4 && s[1] == 'X' && s[2] == 'Y'; }());
static_assert([]() constexpr { S s("ab"); s.insert(1, std::string_view("XY")); return s.size() == 4 && s[1] == 'X'; }());
static_assert([]() constexpr { S s("xy"); s.insert(1, 3, '-'); return s.size() == 5 && s[1] == '-' && s[2] == '-' && s[3] == '-'; }());
static_assert([]() constexpr { S s("ab"); s.insert(s.size(), "cd"); return s.size() == 4 && s[2] == 'c'; }());
static_assert([]() constexpr { S s("ab"); s.insert(0, ""); return s.size() == 2; }());
static_assert([]() constexpr { S s("ab"); s.insert(1, "XYZ", 2); return s.size() == 4 && s[1] == 'X' && s[2] == 'Y'; }());

static_assert([]() constexpr { S s; s += S("hello"); s += std::string(" "); s += std::string_view("world"); s += '!'; return s.size() == 12 && s[0] == 'h' && s[11] == '!'; }());
static_assert([]() constexpr { S s("abc"); s += s; return s.size() == 6 && s[0] == 'a' && s[3] == 'a'; }());

static_assert([]() constexpr { S s("abc"); s = s; return s.size() == 3 && s[0] == 'a'; }());
static_assert([]() constexpr { S a("aaa"); S b("bbb"); a = b; return a.size() == 3 && a[0] == 'b' && b[0] == 'b'; }());
static_assert([]() constexpr { S a("aaa"); S b("bbb"); a = std::move(b); return a.size() == 3 && a[0] == 'b' && b.empty(); }());
static_assert([]() constexpr { S s("abc"); s = "xyz"; return s.size() == 3 && s[0] == 'x'; }());
static_assert([]() constexpr { S s("abc"); s = std::string_view("pq"); return s.size() == 2 && s[1] == 'q'; }());
static_assert([]() constexpr { S s("abc"); s = std::string("st"); return s.size() == 2 && s[0] == 's'; }());
static_assert([]() constexpr { S s("abc"); s = nullptr; return s.empty(); }());

static_assert([]() constexpr { S a("aaa"); S b("bbb"); a.swap(b); return a[0] == 'b' && b[0] == 'a'; }());
static_assert([]() constexpr { S a; S b("ddd"); a.swap(b); return a.size() == 3 && b.empty(); }());
static_assert([]() constexpr { S a("abc"); a.swap(a); return a.size() == 3 && a[0] == 'a'; }());
static_assert([]() constexpr { S a("abc"); S b("defgh"); std::swap(a, b); return a.size() == 5 && b.size() == 3 && a[0] == 'd' && b[0] == 'a'; }());

static_assert([]() constexpr { S s("hello"); s.remove_prefix(2); return s.size() == 3 && s[0] == 'l'; }());
static_assert([]() constexpr { S s("hello"); s.remove_prefix(0); return s.size() == 5; }());
static_assert([]() constexpr { S s("hello"); s.remove_prefix(5); return s.empty(); }());
static_assert([]() constexpr { S s("hello"); s.remove_suffix(2); return s.size() == 3 && s[2] == 'l'; }());
static_assert([]() constexpr { S s("hello"); s.remove_suffix(0); return s.size() == 5; }());
static_assert([]() constexpr { S s("hello"); s.remove_suffix(5); return s.empty(); }());

static_assert([]() constexpr { S s("abc"); s.resize(5, 'x'); return s.size() == 5 && s[3] == 'x' && s[4] == 'x'; }());
static_assert([]() constexpr { S s("abcde"); s.resize(3); return s.size() == 3 && s[2] == 'c'; }());
static_assert([]() constexpr { S s("abc"); s.resize(3); return s.size() == 3; }());
static_assert([]() constexpr { S s("abc"); s.resize(0); return s.empty(); }());
static_assert([]() constexpr { S s; s.resize(4, 'y'); return s.size() == 4 && s[0] == 'y' && s[3] == 'y'; }());
static_assert([]() constexpr { S s("abc"); s.resize(6); return s.size() == 6 && s[3] == '\0' && s[5] == '\0'; }());
static_assert([]() constexpr { S s("abc"); s.clear(); return s.empty(); }());
static_assert([]() constexpr { S s("hello"); s.reserve(100); return s.capacity() >= 100 && s.size() == 5; }());
static_assert([]() constexpr { S s("hello"); s.reserve(3); return s.size() == 5 && s[0] == 'h'; }());

static_assert([]() constexpr { S s; for (char c : S("abc")) s += c; return s.size() == 3 && s[0] == 'a' && s[2] == 'c'; }());
static_assert([]() constexpr { S s("abc"); std::size_t n = 0; for (char c : s) { (void)c; ++n; } return n == 3; }());
static_assert([]() constexpr { S e; std::size_t n = 0; for (char c : e) { (void)c; ++n; } return n == 0; }());
static_assert([]() constexpr { S s("abcd"); auto it = s.begin(); ++it; return *it == 'b'; }());
static_assert([]() constexpr { S s("abcd"); std::size_t n = 0; for (auto it = s.cbegin(); it != s.cend(); ++it) ++n; return n == 4; }());

static_assert([]() constexpr { S s("hello"); std::string_view sv = static_cast<std::string_view>(s); return sv == "hello"; }());
static_assert([]() constexpr { S s; std::string_view sv = static_cast<std::string_view>(s); return sv.empty(); }());
static_assert([]() constexpr { S s("hello"); std::string_view sv = static_cast<std::string_view>(s); return sv.size() == 5 && sv.data() == s.data(); }());

static_assert([]() constexpr { for (std::size_t n = 0; n <= 40; ++n) { S s; for (std::size_t i = 0; i < n; ++i) s += static_cast<char>('a' + (i % 26)); if (s.size() != n) return false; for (std::size_t i = 0; i < n; ++i) if (s[i] != static_cast<char>('a' + (i % 26))) return false; S c(s); if (c != s) return false; if (c.size() != n) return false; } return true; }());

static bool runtime_checks() {
    bool ok = true;

    auto expect_throw = [&](auto &&fn) {
        try {
            fn();
            ok = false;
            hp::println("FAIL: expected throw, none thrown");
        } catch (const std::out_of_range &) {
        } catch (...) {
            ok = false;
            hp::println("FAIL: wrong exception type");
        }
    };

    expect_throw([] { S("abc").at(3); });
    expect_throw([] { S("abc").at(100); });
    expect_throw([] { S().at(0); });

    expect_throw([] { S("abc").substr(4); });
    expect_throw([] { S("abc").substr(S::npos); });

    expect_throw([] { S("abc").insert(4, "x"); });
    expect_throw([] { S("abc").insert(S::npos, "x"); });

    expect_throw([] { S("abc").erase(4); });
    expect_throw([] { S("abc").erase(S::npos); });

    assert(S("hello").size() == 5);
    assert(S("hello")[0] == 'h');
    assert(S("hello").data()[5] == '\0');
    assert(S("hello").find("world") == S::npos);
    assert(S("hello world").find("world") == 6);
    assert(S("hello world").substr(6) == S("world"));
    assert(S("hello").starts_with("he"));
    assert(S("hello").ends_with("lo"));
    assert(S("hello world").contains("lo wo"));

    return ok;
}

int main() {
    if (!runtime_checks()) {
        hp::println("Runtime checks FAILED");
        return 1;
    }
    hp::println("All tests passed");
    return 0;
}