#pragma once

#include <iostream>
#include <windows.h>

namespace hp {

    inline void print(int row, int col, int width, const std::string& msg = "") {
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << msg;
        }

    inline void println(int row, int col, int width, const std::string& msg = "") {
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << msg << std::endl;
        }

    inline void printCenter(int row, const std::string& msg = "") {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int col = ((consoleWidth - msg.length())) / 2;
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << msg;
        std::cout.flush();
        }
    }