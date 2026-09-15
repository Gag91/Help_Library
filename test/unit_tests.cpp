#include <cassert>
#include <filesystem>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "hp/help.hpp"

void test_math() {
    assert(hp::factorial(5) == 120);
    assert(hp::factorialRecursive(5) == 120);
    assert(hp::factorial(0) == 1);
    assert(hp::gcd(-24, 18) == 6);
    assert(hp::lcm(6, 8) == 24);
    assert(hp::fibonacci(10) == 55);
    assert(hp::fibonacciRecursive(10) == 55);

    assert(hp::add(2, 3) == 5);
    assert(hp::add(1, 2, 3) == 6);
    assert(hp::add<int>("10 + 5") == 15);
    assert(hp::sub(8, 3) == 5);
    assert(hp::sub(10, 3, 2) == 5);
    assert(hp::sub<int>("10 - 3") == 7);
    assert(hp::mult(4, 3) == 12);
    assert(hp::mult(2, 3, 4) == 24);
    assert(hp::mult<int>("6 * 7") == 42);
    assert(hp::div(12, 3) == 4);
    assert(hp::div(24, 3, 2) == 4);
    assert(hp::div<int>("24 / 6") == 4);
    assert(hp::calc<int>("7 + 5") == 12);
    assert(hp::calc<int>("7 % 5") == 2);

    assert(hp::math::check::isNumber("-12.5"));
    assert(!hp::math::check::isNumber("."));
    assert(hp::math::make::positive(-5) == 5);
    assert(hp::math::make::negative(5) == -5);
    assert(hp::math::make::Int(4.9) == 4);
    assert(hp::math::make::Float(4) == 4.0f);
}

void test_strings() {
    assert(hp::str::to_upper("Hello") == "HELLO");
    assert(hp::str::to_lower("Hello") == "hello");
    assert(hp::str::trim::white_spaces("a b c") == "abc");
    assert(hp::str::trim::numbers("a1b23") == "123");
    assert(hp::str::trim::letters("a1B2c") == "aBc");
    assert(hp::repeatString("ab", 3) == "ababab");
    assert(hp::starts_with("hello", "hel"));
    assert(hp::ends_with("hello", "llo"));
}

void test_string_edges() {
    assert(hp::str::to_upper("") == "");
    assert(hp::str::to_lower("") == "");
    assert(hp::str::to_upper("Hello, World!") == "HELLO, WORLD!");
    assert(hp::str::to_lower("Hello, World!") == "hello, world!");
    assert(hp::str::trim::white_spaces("") == "");
    assert(hp::str::trim::white_spaces("   ") == "");
    assert(hp::str::trim::numbers("abc") == "");
    assert(hp::str::trim::numbers("") == "");
    assert(hp::str::trim::letters("123") == "");
    assert(hp::str::trim::letters("") == "");
    assert(hp::repeatString("x", 0) == "");
    assert(hp::repeatString("", 5) == "");
    assert(!hp::starts_with("hi", "hello"));
    assert(!hp::ends_with("hi", "hello"));
}

void test_value_types() {
    const hp::BorderChars border = hp::getBorderChars(hp::ASCII);
    assert(border.tl == "+");
    assert(border.hChar == "-");
    assert(hp::getColorCode(hp::RED) == "\033[31m");
    assert(hp::getError(hp::DIV_ZERO).find("Cannot divide") != std::string::npos);

    BigInt first("12345678901234567890");
    BigInt second("10");
    assert((first + second).toString() == "12345678901234567900");
    assert((first / second).toString() == "1234567890123456789");
    assert(BigInt::gcd(BigInt(48), BigInt(18)).toString() == "6");
}

void test_bigint_arithmetic() {
    BigInt zero("0");
    BigInt pos("5");
    BigInt neg("-5");

    assert((zero + zero).toString() == "0");
    assert((pos + zero).toString() == "5");
    assert((pos + neg).toString() == "0");
    assert((neg + pos).toString() == "0");
    assert((neg + neg).toString() == "-10");
    assert((pos - pos).toString() == "0");
    assert((pos - neg).toString() == "10");
    assert((neg - pos).toString() == "-10");
    assert((neg - neg).toString() == "0");

    assert((pos * zero).toString() == "0");
    assert((neg * zero).toString() == "0");
    assert((pos * pos).toString() == "25");
    assert((pos * neg).toString() == "-25");
    assert((neg * neg).toString() == "25");

    assert((pos / pos).toString() == "1");
    assert((pos / neg).toString() == "-1");
    assert((neg / pos).toString() == "-1");
    assert((neg / neg).toString() == "1");

    assert((BigInt("7") % BigInt("3")).toString() == "1");
    assert((BigInt("10") % BigInt("5")).toString() == "0");

    assert((BigInt("2") ^ BigInt("10")).toString() == "1024");
    assert((BigInt("1000000000000") * BigInt("1000000000000")).toString() == "1000000000000000000000000");
}

void test_bigint_edges() {
    BigInt min_int = std::numeric_limits<int>::min();
    BigInt result = min_int / BigInt("-1");
    assert(result.toString() == "2147483648");

    BigInt zero("0");
    assert((zero / BigInt("5")).toString() == "0");

    BigInt a("123");
    BigInt b(std::move(a));
    assert(b.toString() == "123");
    a = BigInt("456");
    assert(a.toString() == "456");

    assert(BigInt("-0").toString() == "0");
    assert(BigInt("+5").toString() == "5");
    assert(BigInt("-5").toString() == "-5");

    assert(BigInt("0") == BigInt("-0"));
    assert(BigInt("5") > BigInt("-5"));
    assert(BigInt("-5") < BigInt("5"));
    assert(BigInt("5") >= BigInt("5"));
    assert(BigInt("5") <= BigInt("5"));
    assert(BigInt("5") != BigInt("6"));

    assert(BigInt::abs(BigInt("-5")).toString() == "5");
    assert(BigInt::sqrt(BigInt("100")).toString() == "10");
    assert(BigInt::sqrt(BigInt("0")).toString() == "0");
    assert(BigInt::gcd(BigInt("12"), BigInt("18")).toString() == "6");
    assert(BigInt::lcm(BigInt("4"), BigInt("6")).toString() == "12");

    BigInt x("5");
    assert((++x).toString() == "6");
    assert((x++).toString() == "6");
    assert(x.toString() == "7");
    assert((--x).toString() == "6");
    assert((x--).toString() == "6");
    assert(x.toString() == "5");
}

void test_bigint_errors() {
    bool threw = false;
    try {
        BigInt a("5");
        BigInt b("0");
        BigInt c = a / b;
        (void)c;
    } catch (const std::runtime_error &) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        BigInt bad("12x3");
    } catch (const std::invalid_argument &) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        BigInt bad("");
    } catch (const std::invalid_argument &) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        BigInt bad("-");
    } catch (const std::invalid_argument &) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        BigInt bad("+");
    } catch (const std::invalid_argument &) {
        threw = true;
    }
    assert(threw);
}

void test_overflow() {
    static_assert(hp::overflow::check::Add(std::numeric_limits<int>::max(), 1));
    static_assert(hp::overflow::check::Add(1500000000, 1500000000));
    static_assert(!hp::overflow::check::Add(5, 5));
    static_assert(!hp::overflow::check::Add(0, 0));

    static_assert(hp::overflow::check::Sub(std::numeric_limits<int>::max(), -1));
    static_assert(!hp::overflow::check::Sub(5, 3));
    static_assert(!hp::overflow::check::Sub(0, 0));

    static_assert(hp::overflow::check::Mult(std::numeric_limits<int>::max(), 2));
    static_assert(!hp::overflow::check::Mult(5, 5));
    static_assert(!hp::overflow::check::Mult(1, 1));

    static_assert(hp::overflow::check::Div(std::numeric_limits<int>::min(), -1));
    static_assert(hp::overflow::check::Div(10, 0));
    static_assert(!hp::overflow::check::Div(10, 2));
}

void test_underflow() {
    static_assert(hp::underflow::check::Add(std::numeric_limits<int>::min(), -1));
    static_assert(!hp::underflow::check::Add(-5, -5));
    static_assert(!hp::underflow::check::Add(-5, 5));

    static_assert(hp::underflow::check::Sub(std::numeric_limits<int>::min(), 1));
    static_assert(!hp::underflow::check::Sub(5, 3));
    static_assert(!hp::underflow::check::Sub(-5, -3));

    static_assert(hp::underflow::check::Mult(-100000, 100000));
    static_assert(!hp::underflow::check::Mult(5, 5));
    static_assert(!hp::underflow::check::Mult(1, 1));

    static_assert(hp::underflow::check::Div(10, 0));
    static_assert(hp::underflow::check::Div(std::numeric_limits<int>::min(), -1));
    static_assert(!hp::underflow::check::Div(10, 2));
}

#ifdef TEST_KNOWN_BUGS
void test_known_bugs() {
    static_assert(!hp::underflow::check::Mult(1, std::numeric_limits<int>::min()));
    static_assert(hp::underflow::check::Mult(2, std::numeric_limits<int>::min()));

    BigInt a("123");
    BigInt b(std::move(a));
    assert(b.toString() == "123");
    assert(a.toString() == "0");
}
#endif

void test_random_and_files() {
    for (int i = 0; i < 20; ++i) {
        const int value = Random::rand(150, 5000);
        assert(value >= 150 && value <= 5000);
    }

    assert(Random::rand(5, 5) == 5);

    bool sawLow = false;
    bool sawHigh = false;
    for (int i = 0; i < 500; ++i) {
        int v = Random::rand(1, 10);
        if (v == 1)
            sawLow = true;
        if (v == 10)
            sawHigh = true;
        if (sawLow && sawHigh)
            break;
    }
    assert(sawLow);
    assert(sawHigh);

    const std::filesystem::path filePath =
        std::filesystem::temp_directory_path() / "help_library_test.txt";
    hp::File file;
    assert(file.write(filePath.string(), "hello"));
    assert(file.exists(filePath.string()));
    assert(file.read(filePath.string()) == "hello");
    assert(file.getSize(filePath.string()) == 5);
    assert(file.f_append(filePath.string(), " world"));
    assert(file.read(filePath.string()) == "hello world");
    assert(file.del(filePath.string()));
    assert(!file.exists(filePath.string()));
}

void test_serializer() {
    const auto filePath =
        std::filesystem::temp_directory_path() / "help_library_serializer_test.txt";
    const std::string path = filePath.string();

    {
        std::vector<std::pair<std::string, int>> data = {{"a", 1}, {"b", 2}, {"c", 3}};
        assert(hp::save<int>(path, data));
    }

    auto a = hp::load<int>(path, "a");
    auto b = hp::load<int>(path, "b");
    auto c = hp::load<int>(path, "c");
    assert(a.has_value() && *a == 1);
    assert(b.has_value() && *b == 2);
    assert(c.has_value() && *c == 3);

    auto missing = hp::load<int>(path, "z");
    assert(!missing.has_value());

    std::filesystem::remove(path);
}

void test_serializer_strings() {
    const auto filePath =
        std::filesystem::temp_directory_path() / "help_library_serializer_strings.txt";
    const std::string path = filePath.string();

    {
        std::vector<std::pair<std::string, std::string>> data = {
            {"name", "Alice"}, {"city", "Paris"}};
        assert(hp::save<std::string>(path, data));
    }

    std::string name;
    std::string city;
    std::vector<std::pair<std::string, std::string *>> toLoad = {
        {"name", &name}, {"city", &city}};
    assert(hp::load<std::string>(path, toLoad));
    assert(name == "Alice");
    assert(city == "Paris");

    std::filesystem::remove(path);
}

void test_serializer_missing_file() {
    auto result = hp::load<int>("/nonexistent/path/does_not_exist.txt", "a");
    assert(!result.has_value());
}

void test_command() {
    std::string out = hp::command("echo hello");
    assert(out.find("hello") != std::string::npos);

    std::string out2 = hp::command("echo world", true);
    assert(out2.find("world") != std::string::npos);

    std::string out3 = hp::command("echo nope", false);
    assert(out3.find("nope") != std::string::npos);
}

void test_concepts() {
    static_assert(hp::Number<int>);
    static_assert(hp::Number<float>);
    static_assert(hp::Number<double>);
    static_assert(!hp::Number<std::string>);
    static_assert(!hp::Number<void *>);

    static_assert(hp::Integer<int>);
    static_assert(hp::Integer<long>);
    static_assert(!hp::Integer<float>);
    static_assert(!hp::Integer<double>);

    static_assert(hp::Float<float>);
    static_assert(hp::Float<double>);
    static_assert(!hp::Float<int>);

    static_assert(hp::Comparable<int>);
    static_assert(hp::Comparable<double>);

    static_assert(hp::Container<std::vector<int>>);
    static_assert(hp::Container<std::string>);
    static_assert(!hp::Container<int>);

    static_assert(hp::Iterable<std::vector<int>>);
    static_assert(hp::Iterable<std::string>);

    static_assert(hp::Printable<int>);
    static_assert(hp::Printable<std::string>);

    static_assert(hp::Hashable<int>);
    static_assert(hp::Hashable<std::string>);
}

void test_type_list() {
    using L = hp::type_list<int, float, char>;
    using E = hp::type_list<>;

    static_assert(L::size == 3);
    static_assert(!L::empty);
    static_assert(E::size == 0);
    static_assert(E::empty);

    static_assert(L::contains<int>);
    static_assert(L::contains<float>);
    static_assert(L::contains<char>);
    static_assert(!L::contains<double>);

    static_assert(L::indexOf<int> == 0);
    static_assert(L::indexOf<float> == 1);
    static_assert(L::indexOf<char> == 2);

    static_assert(L::count<int> == 1);
    static_assert(L::count<float> == 1);

    static_assert(std::is_same_v<L::front<>, int>);
    static_assert(std::is_same_v<L::back<>, char>);
    static_assert(std::is_same_v<L::get<0>, int>);
    static_assert(std::is_same_v<L::get<1>, float>);
    static_assert(std::is_same_v<L::get<2>, char>);

    static_assert(std::is_same_v<L::pop_front, hp::type_list<float, char>>);
    static_assert(std::is_same_v<L::pop_back, hp::type_list<int, float>>);

    static_assert(std::is_same_v<L::append<double>, hp::type_list<int, float, char, double>>);
    static_assert(std::is_same_v<L::prepend<double>, hp::type_list<double, int, float, char>>);

    static_assert(std::is_same_v<L::clear, hp::type_list<>>);
    static_assert(std::is_same_v<E::clear, hp::type_list<>>);
}

void test_overflow_make() {
    static_assert(!hp::overflow::make::Add(1, 2, 3));
    static_assert(hp::overflow::make::Add(1000000000, 1000000000, 1000000000));

    static_assert(!hp::overflow::make::Sub(10, 1, 2));
    static_assert(!hp::overflow::make::Sub(0, -1, -1));

    static_assert(!hp::overflow::make::Mult(2, 3, 4));
    static_assert(hp::overflow::make::Mult(1000000, 1000000, 1000000));
}

void test_underflow_make() {
    static_assert(!hp::underflow::make::Add(-1, -2, -3));
    static_assert(hp::underflow::make::Add(-1000000000, -1000000000, -1000000000));

    static_assert(!hp::underflow::make::Sub(-5, 1, 2));
    static_assert(hp::underflow::make::Sub(-1000000000, 1000000000, 1000000000));

    static_assert(!hp::underflow::make::Mult(-2, 3, 4));
    static_assert(hp::underflow::make::Mult(-1000000, 1000000, 1000000));
}

int main() {
    test_math();
    test_strings();
    test_string_edges();
    test_value_types();
    test_bigint_arithmetic();
    test_bigint_edges();
    test_bigint_errors();
    test_overflow();
    test_underflow();
    test_overflow_make();
    test_underflow_make();
    test_concepts();
    test_type_list();
    test_serializer();
    test_serializer_strings();
    test_serializer_missing_file();
    test_command();
    test_random_and_files();

    std::cout << "All unit tests passed.\n";
    return 0;
}