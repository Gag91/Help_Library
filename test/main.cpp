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

    std::string ageStr = "";
    std::string randStr = "";
    hp::load("test.txt",{{"name",&user.name},{"age",&ageStr},{"random",&randStr}}); 

    user.age = std::stoi(ageStr);
    user.random = std::stoi(randStr);
    hp::centeredBox(3, 50, user.name);
    hp::centeredBox(6, 50, std::to_string(user.age));
    hp::centeredBox(9, 50, std::to_string(user.random));


    }

