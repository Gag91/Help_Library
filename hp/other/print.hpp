#pragma once

#include "hp/platform/console.hpp"

#include <iostream>
#include <string>

namespace hp {

    inline void print(int row, int col, int width, const std::string &msg = "") {
        (void)width;
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << msg;
    }

    inline void println(int row, int col, int width, const std::string &msg = "") {
        (void)width;
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << msg << std::endl;
    }

    inline void printCenter(int row, const std::string &msg = "") {
        int consoleWidth = hp::platform::console_width();
        int col = (consoleWidth - static_cast<int>(msg.length())) / 2;
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << msg;
        std::cout.flush();
    }
} // namespace hp