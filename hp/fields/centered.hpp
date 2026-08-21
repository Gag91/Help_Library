#pragma once

#include <iostream>
#include <vector>           
#include <limits>           
#include <windows.h>
#include <conio.h>
#include "hp/colors/color.hpp"
#include "hp/borderStyle/border.hpp"
#include "hp/fields/field.hpp"     
#include "hp/keyboard/keyboard.hpp"
#include "hp/time/time.hpp"

namespace hp {

    // ----- Centered Box | Use of Windows API | Use of ANSCII color from hp/colors/color.hpp | Use of ASCII Border style from hp/borderStyle/border.hpp
    inline void centeredBox(int row, int width, const std::string& msg, hp::Color color = hp::WHITE, BorderStyle style = EXTENDED) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        if (width == -1) {
            width = msg.size() + 4;
            }

        int col = (consoleWidth - width) / 2;
        std::string tl, tr, bl, br, sep, hChar, vChar;
        switch (style) {
            case ASCII:
                tl = "+"; tr = "+"; bl = "+"; br = "+";
                sep = "+"; hChar = "-"; vChar = "|";
                break;
            case EXTENDED:
                tl = "┌"; tr = "┐"; bl = "└"; br = "┘";
                sep = "├"; hChar = "─"; vChar = "│";
                break;
            case DOUBLE:
                tl = "╔"; tr = "╗"; bl = "╚"; br = "╝";
                sep = "╠"; hChar = "═"; vChar = "║";
                break;
            }
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << tl << repeatString(hChar, width) << tr;
        std::cout << "\033[" << (row + 1) << ";" << col << "H";
        std::cout << vChar << getColorCode(color) << msg << getColorCode(RESET) << std::string((width - msg.size()), ' ') << vChar;
        std::cout << "\033[" << (row + 2) << ";" << col << "H";
        std::cout << bl << repeatString(hChar, width) << br;
        }


    inline std::string CenteredInput(int row, int width, const std::string& msg, std::string allowed = "", hp::Color color = hp::WHITE, int lenght = -1, BorderStyle style = EXTENDED) {
        if (lenght == -1) lenght = width;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        if (width < msg.length() + 2) {
            width = msg.length() + 4;
            }
        int col = (consoleWidth - width) / 2;
        std::string input;
        std::string prevInput = " ";
        std::string tl, tr, bl, br, sep, hChar, vChar;
        switch (style) {
            case ASCII:
                tl = "+"; tr = "+"; bl = "+"; br = "+";
                sep = "+"; hChar = "-"; vChar = "|";
                break;
            case EXTENDED:
                tl = "┌"; tr = "┐"; bl = "└"; br = "┘";
                sep = "├"; hChar = "─"; vChar = "│";
                break;
            case DOUBLE:
                tl = "╔"; tr = "╗"; bl = "╚"; br = "╝";
                sep = "╠"; hChar = "═"; vChar = "║";
                break;
            }
        while (true) {
            if (input != prevInput) {
                std::cout << "\033[" << row << ";" << col << "H";
                std::cout << getColorCode(hp::WHITE);
                std::cout << tl << repeatString(hChar, width) << tr;
                std::cout << "\033[" << (row + 1) << ";" << col << "H";
                std::cout << vChar;
                if (input.empty()) {
                    std::cout << getColorCode(hp::BRIGHT_BLACK) << msg
                        << getColorCode(hp::WHITE);
                    std::cout << std::string(width - msg.size(), ' ');
                    }
                else {
                    std::cout << getColorCode(color) << input
                        << getColorCode(hp::WHITE);
                    std::cout << std::string(width - input.size(), ' ');
                    }
                std::cout << vChar;
                std::cout << "\033[" << (row + 2) << ";" << col << "H";
                std::cout << bl << repeatString(hChar, width) << br;
                std::cout << getColorCode(hp::RESET);
                std::cout.flush();
                prevInput = input;
                }
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 13) {
                    return input;
                    }
                else if (ch == 8) {
                    if (!input.empty()) {
                        input.pop_back();
                        }
                    }
                else if (ch == 27) {
                    return "";
                    }
                else if (ch >= 32 && ch <= 126 && input.size() < lenght) {
                    char c = static_cast<char>(ch);
                    if (allowed.empty() || allowed.find(c) != std::string::npos) {
                        input += c;
                        }
                    }
                }
            Sleep(10);
            }
        }

    // ----- Center Field Menu navigateble using arrow keys
    inline int CenteredFieldMenu(int startRow, int width, const std::vector<std::string> options, hp::Color color = hp::WHITE, BorderStyle style = EXTENDED) {
        while (_kbhit()) _getch();
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int col = (consoleWidth - width) / 2;
        if (col < 1) col = 1;
        int index = 0;
        int prevIndex = -1;
        int maxIndex = static_cast<int>(options.size()) - 1;
        for (int i = 0; i < options.size(); i++) {
            int row = startRow + (i * 3);
            hp::Field(row, col, width, options[i], hp::WHITE, true, style);
            }
        while (true) {
            hp::updateKeyboard();
            if (hp::KeyIsPressed::Up) {
                index = (index > 0) ? index - 1 : maxIndex;
                }
            else if (hp::KeyIsPressed::Down) {
                index = (index < maxIndex) ? index + 1 : 0;
                }
            else if (hp::KeyIsPressed::Enter) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return index;
                }
            else if (hp::KeyIsPressed::Escape) {
                return -1;
                }
            if (index != prevIndex) {
                if (prevIndex != -1) {
                    int prevRow = startRow + (prevIndex * 3);
                    hp::Field(prevRow, col, width, options[prevIndex], hp::WHITE, true, style);
                    }
                int currentRow = startRow + (index * 3);
                hp::Field(currentRow, col, width, options[index], color, true, style);
                prevIndex = index;
                }
            hp::wait(0.05);
            }
        }

    inline void clearCenteredInput(int row, int width) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int col = (consoleWidth - width) / 2;
        for (int i = 0; i < 3; i++) {
            std::cout << "\033[" << (row + i) << ";" << col << "H";
            std::cout << "\033[2K";
            }
        }
    }