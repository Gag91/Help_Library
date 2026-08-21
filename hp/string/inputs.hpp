#pragma once
#include <iostream>

namespace hp {

    // ---- Most regular getInput
    inline std::string getInput(const std::string& msg) {
        std::string input;
        std::cout << msg;
        std::cin >> input;
        return input;
        }
    // ---- More Advanced function to gte the input of the user
    template <typename T>
    T get(const std::string& msg, const std::string& error = "") {
        T value;
        while (true) {
            if (!msg.empty()) std::cout << msg;
            if (std::cin >> value) {
                return value;
                }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << error << std::endl;
            }
        }

    // ---- Same as hp::get but more advanced for string | Using getline
    inline std::string getStr(const std::string& msg, const std::string& error = "") {
        std::string value;
        std::cout << msg;
        if (std::getline(std::cin, value)) { // <-- here is the diffrence whit hp::get
            return value;
            }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max());
            std::cout << error << std::endl;
            }
        return "";
        }

    }