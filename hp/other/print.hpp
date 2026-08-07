#pragma once

#include <iostream>
#include <windows.h>

namespace hp {

    void print(int row, int col, int width, const std::string& msg = "") {
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << msg;
        }

    void println(int row, int col, int width, const std::string& msg = "") {
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << msg << std::endl;
        }

    void printCenter(int row, int width, const std::string& msg = "") {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int col = ((consoleWidth - msg.length()) + 4) / 2;
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << msg;
        std::cout.flush();
        }
    }