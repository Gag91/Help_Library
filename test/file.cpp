#include "hp/help.hpp"
#include <iostream>
#include <memory>
#include <meta>
#include <string>

struct Bob2 {
    int a, b, c, d, e, f;
};

struct Bob {
    int a, b, c, d, e, f;
    Bob2 bob2;
};

struct User {
    std::string name;
    int age = 0;
    std::unique_ptr<std::string> ptr;
    std::unique_ptr<int> iptr;
    char c = ' ';
    double a = 0.0;
    Bob bob;
};

int main() {
    hp::cls();

    /*User user{
        "Xavi\n",
        50,
        std::make_unique<std::string>("hi"),
        std::make_unique<int>(1337),
        'Z',
        3.14159};

    hp::save("test.txt", user);*/

    Bob b = hp::load<Bob>("test.txt", "bob");

    std::println("we got bob: {} {} {} {} {} {}", b.a, b.b, b.c, b.d, b.e, b.f);

    /*std::cout << "User name: '" << user.name << "'\n";
    std::cout << "User age: '" << user.age << "'\n";
    std::cout << "User char: '" << user.c << "'\n";
    std::cout << "User double: '" << user.a << "'\n";

    if (user.iptr) {
        std::cout << "User iptr: '" << *(user.iptr) << "'\n";
    } else {
        std::cout << "User iptr: 'nullptr'\n";
    }

    if (user.ptr) {
        std::cout << "User ptr: '" << *(user.ptr) << "'\n";
    } else {
        std::cout << "User ptr: 'nullptr'\n";
    }

    std::cout << "User Bob: " << user.bob.a << " : " << user.bob.b << " : " << user.bob.c << " : " << user.bob.d << " : " << user.bob.e << " : " << user.bob.f << "\n";
    std::cout << "User Bob Bob2: " << user.bob.bob2.a << " : " << user.bob.bob2.b << " : " << user.bob.bob2.c << " : " << user.bob.bob2.d << " : " << user.bob.bob2.e << " : " << user.bob.bob2.f << "\n";*/

    return 0;
}
