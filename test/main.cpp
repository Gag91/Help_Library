#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "hp/help.hpp"

struct User {
    std::string name;
    int age;
    int random;
    };
User user;

int main() {
    hp::cls();
    hp::enableUTF8();

    user.name = hp::load("test.txt", "name").value();
    user.age = hp::load<int>("test.txt", "age").value();
    user.random = hp::load<int>("test.txt", "random").value();

    hp::centeredBox(3, 50, user.name);
    hp::centeredBox(6, 50, std::to_string(user.age));
    hp::centeredBox(9, 50, std::to_string(user.random));


    }

