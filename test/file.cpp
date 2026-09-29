#include "hp/help.hpp"
#include <iostream>
#include <memory>
#include <meta>
#include <string>

enum Color {
    Red,
    Green,
    Blue
};

struct User {
    std::string name;
    int age = 0;
    std::unique_ptr<std::string> ptr;
    std::unique_ptr<int> iptr;
    char c = ' ';
    double a = 0.0;
    Color color;
};

int main() {
    hp::cls();

    User user{
        "Xavi\n",
        50,
        std::make_unique<std::string>("hi"),
        std::make_unique<int>(1337),
        'Z',
        3.14159};

    hp::save("test.txt", user);
    Color c;
    hp::save<Color>("enum.txt", c);

    return 0;
}
