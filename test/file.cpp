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
    std::vector<std::string> strs = {"world", "hello", "xavi", "yesyes", "kek"};
};

struct Point {
    int x;
    int y;
};
struct Line {
    Point start;
    Point end;
};

int main() {

    User user{
        "Xavi\n",
        50,
        std::make_unique<std::string>("hi"),
        std::make_unique<int>(1337),
        'Z',
        3.14159};

    Color c;
    User u;
    Line l{{1, 2}, {3, 4}};
    hp::ser::to_json_file("jsonTest.json", user);
    auto ll = hp::ser::to_json_pretty(user);
    std::println("Pretty Json: {}", hp::ser::to_json_pretty(user));
    hp::save("line.txt", l);

    Line loaded;
    hp::load("line.txt", loaded);
    std::println("start: ({}, {})", loaded.start.x, loaded.start.y);
    std::println("end:   ({}, {})", loaded.end.x, loaded.end.y);

    if (!hp::load("test.txt", u)) {
        std::println("Not founded");
    }
    std::println("Enum Value: {}", hp::str::to_string(u.color));

    return 0;
}
