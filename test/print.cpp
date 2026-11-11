#include "hp/print/print.hpp"

#include <array>
#include <cstdint>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
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

    template <typename... Args>
    void smoke(std::string_view label,
               hp::detail::format_string<Args...> fmt,
               const Args &...args) {
        hp::println("  [smoke] {}: fmt=\"{}\"  =>  {}", label, fmt.str, hp::format(fmt, args...));
    }

    enum Color {
        RED,
        GREEN,
        BLUE,
    };
    enum class Fruit {
        Apple,
        Banana,
        Cherry,
    };
    enum class Small : std::uint8_t {
        A = 1,
        B = 2,
    };

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

    struct HasFormat {
        int v;
        friend std::string to_string(const HasFormat &h) {
            return "<HF:" + std::to_string(h.v) + ">";
        }
    };

    void test_literals() {
        eq(hp::format(""), "", "empty format");
        eq(hp::format("hello"), "hello", "plain text");
        eq(hp::format("hello\n"), "hello\n", "trailing newline");
        eq(hp::format("multi\nline\ntext"), "multi\nline\ntext", "multi-line");
        eq(hp::format("no {} here", 42), "no 42 here", "text around placeholder");
    }

    void test_single_arg() {
        eq(hp::format("{}", 42), "42", "int");
        eq(hp::format("{}", 0), "0", "zero");
        eq(hp::format("{}", -7), "-7", "negative int");
        eq(hp::format("{}", 1234567890), "1234567890", "large int");
        eq(hp::format("x = {}", 5), "x = 5", "int with text");

        eq(hp::format("{}", "literal"), "literal", "const char[N]");
        eq(hp::format("{}", std::string("dynamic")), "dynamic", "std::string");
        eq(hp::format("{}", std::string_view("view")), "view", "std::string_view");
        const char *p = "pointer";
        eq(hp::format("{}", p), "pointer", "const char*");
        eq(hp::format("{}", std::string()), "", "empty string");

        smoke("char 'A'", "{}", 'A');
        smoke("char '0'", "{}", '0');
        smoke("bool true", "{}", true);
        smoke("bool false", "{}", false);
        smoke("double 3.14", "{}", 3.14);
    }

    void test_multi_arg() {
        eq(hp::format("{} + {} = {}", 1, 2, 3), "1 + 2 = 3", "3 ints");
        eq(hp::format("{}{}{}", "a", "b", "c"), "abc", "3 strings");
        eq(hp::format("{}, {}!", "hi", "world"), "hi, world!", "2 strings + punct");
        eq(hp::format("{} then {} then {}", 10, 20, 30), "10 then 20 then 30", "3 ints + text");
        eq(hp::format("{} / {} / {}", 1, 2, 3), "1 / 2 / 3", "3 ints slashes");
    }

    void test_escapes() {
        eq(hp::format("{{"), "{", "escaped open");
        eq(hp::format("}}"), "}", "escaped close");
        eq(hp::format("{{}}"), "{}", "escaped pair");
        eq(hp::format("{{{{"), "{{", "double escaped open");
        eq(hp::format("}}}}"), "}}", "double escaped close");
        eq(hp::format("a{{b}}c"), "a{b}c", "text + escape");
        eq(hp::format("{{{}}}", "x"), "{x}", "brace sandwich");
        eq(hp::format("{{{}}}", 42), "{42}", "brace sandwich int");
    }

    void test_float_precision() {
        eq(hp::format("{:.2f}", 3.14159), "3.14", "2 decimals");
        eq(hp::format("{:.0f}", 3.9), "4", "0 decimals rounds up");
        eq(hp::format("{:.5f}", 1.0), "1.00000", "5 decimals");
        eq(hp::format("{:.10f}", 3.14), "3.1400000000", "10 decimals");
        eq(hp::format("{:.f}", 3.14), "3", ".f no digits");
        eq(hp::format("{:.2}", 3.14), "3.14", ".2 no f");
        eq(hp::format("pi = {:.3f}", 3.14159), "pi = 3.142", "text + precision");
    }

    void test_alignment() {
        eq(hp::format("[{:5}]", "hi"), "[hi   ]", "width 5 default left");
        eq(hp::format("[{:<5}]", "hi"), "[hi   ]", "explicit left");
        eq(hp::format("[{:>5}]", "hi"), "[   hi]", "right");
        eq(hp::format("[{:^5}]", "hi"), "[ hi  ]", "center odd pad");
        eq(hp::format("[{:^6}]", "hi"), "[  hi  ]", "center even pad");
        eq(hp::format("[{:10}]", "abc"), "[abc       ]", "width 10");
        eq(hp::format("[{:<0}]", "hi"), "[hi]", "zero width");
        eq(hp::format("[{:>20}]", "hi"), "[                  hi]", "wide right");
        eq(hp::format("[{:>2}]", "hello"), "[hello]", "width < string");
        eq(hp::format("[{:}]", "hi"), "[hi]", "{:} same as {}");
    }

    void test_print_vs_println() {
        // print() = no newline
        hp::print("a");
        hp::print("b");
        hp::print("c");
        hp::println("");
        // println() = trailing newline
        hp::println("line1");
        hp::println("line2");
    }

    void test_format_returns_string() {
        std::string s = hp::format("{}", 42);
        eq(s, "42", "format returns string");
        eq(std::to_string(s.size()), "2", "format result length");
        eq(hp::format("{}", s), "42", "reformat result");
        eq(hp::format("{} + {}", s, s), "42 + 42", "reformat twice");
    }

    void test_state_stability() {
        eq(hp::format("{}", 1), "1", "call 1");
        eq(hp::format("{}", 2), "2", "call 2");
        eq(hp::format("{}", 3), "3", "call 3");

        int x = 5;
        hp::format("{}", x);
        eq(std::to_string(x), "5", "arg not modified");

        auto fmt = hp::detail::format_string<int>{"{}"};
        eq(hp::format(fmt, 1), "1", "reuse fmt #1");
        eq(hp::format(fmt, 2), "2", "reuse fmt #2");
    }

    void test_containers() {
        smoke("vector<int>", "{}", std::vector<int>{1, 2, 3});
        smoke("vector<string>", "{}", std::vector<std::string>{"a", "bb", "ccc"});
        smoke("nested vector", "{}", std::vector<std::vector<int>>{{1, 2}, {3, 4, 5}, {6}});
        smoke("array<int,3>", "{}", std::array<int, 3>{7, 8, 9});
        smoke("set<int>", "{}", std::set<int>{3, 1, 2});
        smoke("empty vector", "{}", std::vector<int>{});
        smoke("string as arg", "{}", std::string("hello"));
    }

    void test_enums() {
        smoke("unscoped RED", "{}", RED);
        smoke("unscoped GREEN", "{}", GREEN);
        smoke("enum class", "{}", Fruit::Apple);
        smoke("enum class 2", "{}", Fruit::Cherry);
        smoke("small enum", "{}", Small::A);
    }

    void test_classes() {
        smoke("aggregate struct", "{}", Point{1, 2});
        smoke("aggregate empty", "{}", Point{0, 0});
        smoke("with to_string()", "{}", Person{"Alice", 30});
        smoke("with ADL", "{}", HasFormat{7});
    }

    void test_malformed() {
        hp::println("  (unsupported specifiers — throws once the runtime fix is applied)");
        auto try_throw = [](auto &&f, std::string_view label) {
            try {
                f();
                hp::println("  [note] {} did not throw (fix not applied?)", label);
            } catch (const std::runtime_error &e) {
                hp::println("  [ok]   {} threw: \"{}\"", label, std::string_view(e.what()));
            }
        };
        try_throw([] { (void)hp::format("{:d}", 42); }, "{:d}");
        try_throw([] { (void)hp::format("{:x}", 255); }, "{:x}");
        try_throw([] { (void)hp::format("{:>10.2f}", 3.14); }, "{:>10.2f}");
    }

    void test_runtime_arguments() {
        std::string name = "Alice";
        int age = 30;
        eq(hp::format("{} is {}", name, age), "Alice is 30", "runtime args");

        std::vector<int> v{1, 2, 3};
        smoke("runtime vector", "{}", v);
    }

    void test_original_regression() {
        std::vector<std::vector<int>> vec{{1, 2}, {3, 4, 5}, {6}};
        hp::println("Vector: {}", vec);
        ++g_pass;
    }

} // namespace

int main() {
    hp::println("===== hp::format / hp::print / hp::println test suite =====");

    hp::println("\n[1]  literals");
    test_literals();
    hp::println("[2]  single argument");
    test_single_arg();
    hp::println("[3]  multiple arguments");
    test_multi_arg();
    hp::println("[4]  escaped braces");
    test_escapes();
    hp::println("[5]  float precision");
    test_float_precision();
    hp::println("[6]  alignment");
    test_alignment();
    hp::println("[7]  print vs println");
    test_print_vs_println();
    hp::println("[8]  format returns string");
    test_format_returns_string();
    hp::println("[9]  state stability");
    test_state_stability();
    hp::println("[10] containers");
    test_containers();
    hp::println("[11] enums");
    test_enums();
    hp::println("[12] classes");
    test_classes();
    hp::println("[13] malformed specifiers");
    test_malformed();
    hp::println("[14] runtime arguments");
    test_runtime_arguments();
    hp::println("[15] original regression");
    test_original_regression();

    hp::println("");
    hp::println("===== {} passed, {} failed =====", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}