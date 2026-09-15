#include "hp/help.hpp"
#include <string>

struct User {
    std::string name;
    int age;
};

int main() {
    User user;

    hp::cls();
    hp::save<std::string>("test.txt", {{"name", "Alice"}, {"city", "Paris"}});
    if (!hp::load<std::string>("u.txt", {{"name", &user.name}})) {
        std::cout << "Cannot open file u.txt";
    }

    hp::centeredBox(3, 50, user.name);
}