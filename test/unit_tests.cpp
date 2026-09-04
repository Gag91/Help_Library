#include <cassert>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "hp/bigInt/bigint.hpp"
#include "hp/borderStyle/border.hpp"
#include "hp/errors/error.hpp"
#include "hp/math/math.hpp"
#include "hp/other/other.hpp"
#include "hp/random/random.hpp"
#include "hp/string/string.hpp"

void test_math()
{
    assert(hp::factorial(5) == 120);
    assert(hp::factorialRecursive(5) == 120);
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

void test_strings()
{
    assert(hp::str::to_upper("Hello") == "HELLO");
    assert(hp::str::to_lower("Hello") == "hello");
    assert(hp::str::trim::white_spaces("a b c") == "abc");
    assert(hp::str::trim::numbers("a1b23") == "123");
    assert(hp::str::trim::letters("a1B2c") == "aBc");
    assert(hp::repeatString("ab", 3) == "ababab");
    assert(hp::starts_with("hello", "hel"));
    assert(hp::ends_with("hello", "llo"));
}

void test_value_types()
{
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

void test_random_and_files()
{
    for (int i = 0; i < 20; ++i)
    {
        const int value = Random::rand(150, 5000);
        assert(value >= 150 && value <= 5000);
    }

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

int main()
{
    test_math();
    test_strings();
    test_value_types();
    test_random_and_files();

    std::cout << "All unit tests passed.\n";
    return 0;
}