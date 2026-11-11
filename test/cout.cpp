#include "hp/print/print.hpp"

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

    int g_pass = 0;
    int g_fail = 0;

    void eq(std::string_view got, std::string_view want, std::string_view label) {
        if (got == want) {
            ++g_pass;
            return;
        }
        ++g_fail;
        hp::println("");
        hp::println("[FAIL] {}", label);
        hp::println("       expected: \"{}\"", want);
        hp::println("       actual:   \"{}\"", got);
    }

    template <typename F>
    std::string captured(F &&fn) {
        std::ostringstream oss;
        hp::detail::OStream out{oss};
        fn(out);
        return oss.str();
    }

    struct Point {
        int x;
        int y;
    };

    struct Person {
        std::string name;
        int age;
        std::string to_string() const {
            return name + " (" + std::to_string(age) + ")";
        }
    };

    struct Widget {
        int v;
    };

    std::ostream &operator<<(std::ostream &os, const Widget &w) {
        return os << "Widget{" << w.v << "}";
    }

    void test_native_scalars() {
        eq(captured([](auto &o) { o << 42; }), "42", "int 42");
        eq(captured([](auto &o) { o << 0; }), "0", "int 0");
        eq(captured([](auto &o) { o << -7; }), "-7", "int -7");
        eq(captured([](auto &o) { o << 255u; }), "255", "unsigned int");
        eq(captured([](auto &o) { o << 100LL; }), "100", "long long");
        eq(captured([](auto &o) { o << 'A'; }), "A", "char 'A'");
        eq(captured([](auto &o) { o << '0'; }), "0", "char '0'");

        eq(captured([](auto &o) { o << true; }), "1", "bool true (native, not \"true\")");
        eq(captured([](auto &o) { o << false; }), "0", "bool false (native, not \"false\")");

        eq(captured([](auto &o) { o << 3.14; }), "3.14", "double 3.14 (shortest repr)");
        eq(captured([](auto &o) { o << 1.0f; }), "1", "float 1.0");
    }

    void test_native_strings() {
        eq(captured([](auto &o) { o << "hello"; }), "hello", "char[6]");
        eq(captured([](auto &o) { o << "literal"; }), "literal", "char[8]");
        eq(captured([](auto &o) { o << std::string("dyn"); }), "dyn", "std::string");
        eq(captured([](auto &o) { o << std::string_view("view"); }), "view", "std::string_view");
        eq(captured([](auto &o) { o << std::string(); }), "", "empty std::string");

        const char *p = "ptr";
        eq(captured([&p](auto &o) { o << p; }), "ptr", "const char*");

        char buf[4] = {'b', 'u', 'f', '\0'};
        eq(captured([&buf](auto &o) { o << buf; }), "buf", "char[4] buffer");
    }

    void test_fallback_containers() {
        eq(captured([](auto &o) { o << std::vector<int>{1, 2, 3}; }),
           "[1, 2, 3]", "vector<int>");

        eq(captured([](auto &o) { o << std::vector<int>{}; }),
           "[]", "empty vector");

        eq(captured([](auto &o) {
               o << std::vector<std::vector<int>>{{1, 2}, {3, 4, 5}, {6}};
           }),
           "[[1, 2], [3, 4, 5], [6]]", "nested vector");

        eq(captured([](auto &o) {
               o << std::vector<std::string>{"a", "bb", "ccc"};
           }),
           "[a, bb, ccc]", "vector<string>");
    }

    void test_fallback_pairs() {
        eq(captured([](auto &o) { o << std::pair<int, int>{1, 2}; }),
           "(1: 2)", "pair<int,int>");

        eq(captured([](auto &o) { o << std::pair<std::string, int>{"x", 3}; }),
           "(x: 3)", "pair<string,int>");
    }

    void test_fallback_aggregates() {
        eq(captured([](auto &o) { o << Point{1, 2}; }), "{1, 2}", "aggregate Point{1,2}");
        eq(captured([](auto &o) { o << Point{0, 0}; }), "{0, 0}", "aggregate Point{0,0}");
    }

    void test_fallback_member_to_string() {
        eq(captured([](auto &o) { o << Person{"Alice", 30}; }),
           "Alice (30)", "member to_string non-zero");

        eq(captured([](auto &o) { o << Person{"Bob", 0}; }),
           "Bob (0)", "member to_string zero");
    }

    void test_native_wins_over_fallback() {
        eq(captured([](auto &o) { o << Widget{7}; }),
           "Widget{7}", "user operator<< overrides to_string");
    }

    void test_manipulators() {
        eq(captured([](auto &o) { o << std::hex << 255; }),
           "ff", "std::hex 255");

        eq(captured([](auto &o) { o << std::hex << 255 << ' ' << 16; }),
           "ff 10", "std::hex sticky");

        eq(captured([](auto &o) { o << std::oct << 8; }),
           "10", "std::oct");

        eq(captured([](auto &o) { o << std::boolalpha << true; }),
           "true", "std::boolalpha -> true");

        eq(captured([](auto &o) { o << std::showpos << 42; }),
           "+42", "std::showpos");

        eq(captured([](auto &o) { o << std::setw(8) << 42; }),
           "      42", "std::setw(8)");

        eq(captured([](auto &o) { o << std::setw(8) << std::setfill('0') << 42; }),
           "00000042", "setw(8) + setfill('0')");

        eq(captured([](auto &o) { o << std::fixed << std::setprecision(2) << 3.14159; }),
           "3.14", "fixed + setprecision(2)");

        eq(captured([](auto &o) { o << std::defaultfloat << std::setprecision(3) << 3.14159; }),
           "3.14", "defaultfloat + setprecision(3)");
    }

    void test_endl_flush() {
        eq(captured([](auto &o) { o << "line" << std::endl; }),
           "line\n", "std::endl writes newline");

        eq(captured([](auto &o) { o << "line\n"; }),
           "line\n", "'\\n' literal");

        eq(captured([](auto &o) { o << "x" << std::flush; }),
           "x", "std::flush (no visible output)");

        eq(captured([](auto &o) { o << "a" << std::endl
                                    << "b" << std::endl; }),
           "a\nb\n", "multiple endl");
    }

    void test_format_interop() {
        // hp::format returns std::string → native path
        eq(captured([](auto &o) { o << hp::format("x = {}", 42); }),
           "x = 42", "format -> cout");

        eq(captured([](auto &o) { o << hp::format("{:.2f}", 3.14159); }),
           "3.14", "format float");

        eq(captured([](auto &o) { o << hp::format("{} + {} = {}", 1, 2, 3) << '\n'; }),
           "1 + 2 = 3\n", "format + newline");

        eq(captured([](auto &o) {
               o << "prefix: " << hp::format("x={}", 5)
                 << " vec=" << std::vector<int>{1, 2}
                 << " suffix";
           }),
           "prefix: x=5 vec=[1, 2] suffix", "mixed format + stream + container");
    }

    void test_chaining() {
        eq(captured([](auto &o) { o << 1 << 2 << 3; }), "123", "chained ints");

        eq(captured([](auto &o) {
               o << "a=" << 1 << ", b=" << 2 << ", c=" << std::vector<int>{3, 4};
           }),
           "a=1, b=2, c=[3, 4]", "chained mixed native/fallback");
    }

    void test_native_accessor() {
        std::ostringstream oss;
        hp::detail::OStream out{oss};

        out.native() << std::hex << 255;
        eq(oss.str(), "ff", ".native() == underlying stream");

        eq(&out.native() == &out.native() ? "same" : "diff", "same", ".native() stable");
    }

    void test_real_cout() {
        hp::cout << "[smoke] hp::cout native:    " << 42 << '\n';
        hp::cout << "[smoke] hp::cout fallback:  " << std::vector<int>{1, 2, 3} << '\n';
        hp::cout << "[smoke] hp::cout aggregate: " << Point{1, 2} << '\n';
        hp::cout << "[smoke] hp::cout format:    " << hp::format("x={}, y={}", 3, 4) << '\n';
        hp::cout << "[smoke] hp::cout hex:       " << std::hex << 255 << std::dec << '\n';
        hp::cout << std::endl;

        hp::cerr << "[smoke] hp::cerr writes to stderr" << std::endl;
        hp::clog << "[smoke] hp::clog writes to stderr (buffered)" << std::endl;
    }

} // namespace

int main() {
    hp::println("===== hp::cout test suite =====");

    hp::println("\n[1]  native scalars");
    test_native_scalars();
    hp::println("[2]  native strings");
    test_native_strings();
    hp::println("[3]  fallback containers");
    test_fallback_containers();
    hp::println("[4]  fallback pairs");
    test_fallback_pairs();
    hp::println("[5]  fallback aggregates");
    test_fallback_aggregates();
    hp::println("[6]  fallback member_to_string");
    test_fallback_member_to_string();
    hp::println("[7]  native wins over fallback");
    test_native_wins_over_fallback();
    hp::println("[8]  manipulators");
    test_manipulators();
    hp::println("[9]  endl / flush");
    test_endl_flush();
    hp::println("[10] hp::format interop");
    test_format_interop();
    hp::println("[11] chaining");
    test_chaining();
    hp::println("[12] .native() accessor");
    test_native_accessor();
    hp::println("[13] real hp::cout (smoke)");
    test_real_cout();

    hp::println("");
    hp::println("===== {} passed, {} failed =====", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}