#include "hp/help.hpp"
#include <iostream>
#include <memory>
#include <string>

struct User {
    std::string name;
    int age = 0;
    std::unique_ptr<std::string> ptr;
    std::unique_ptr<int> iptr;
    char c = ' ';
    double a = 0.0;
};

int main() {
    hp::cls();

    User user{
        "Xavi",
        50,
        std::make_unique<std::string>("hi"),
        std::make_unique<int>(1337),
        'Z',
        3.14159};

    hp::save("test.txt", user);

    user = User{};
    hp::load("test.txt", user);

    std::cout << "User name: '" << user.name << "'\n";
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

    return 0;
}
