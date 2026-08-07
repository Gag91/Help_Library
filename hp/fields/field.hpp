#pragma once

#include <iostream>
#include <string>
#include "hp/colors/color.hpp"
#include "hp/borderStyle/border.hpp"
#include "hp/string/string.hpp"   // For repeatString

namespace hp {

    // ----- Text Field -----
    inline void Field(int row, int col, int width, const std::string& msg,
        Color color = WHITE, bool all = false,
        BorderStyle style = EXTENDED) {
        std::string display = msg;
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
        if (display.size() > width) {
            display = display.substr(0, width);
            }
        std::cout << "\033[" << row << ";" << col << "H";
        if (all) {
            std::cout << getColorCode(color);
            std::cout << tl << repeatString(hChar, width) << tr;
            std::cout << "\033[" << (row + 1) << ";" << col << "H";
            std::cout << vChar << display << std::string(width - display.size(), ' ') << vChar;
            std::cout << "\033[" << (row + 2) << ";" << col << "H";
            std::cout << bl << repeatString(hChar, width) << br;
            }
        else {
            std::cout << getColorCode(WHITE);
            std::cout << tl << repeatString(hChar, width) << tr;
            std::cout << "\033[" << (row + 1) << ";" << col << "H";
            std::cout << vChar << getColorCode(color) << display << getColorCode(WHITE);
            std::cout << std::string(width - display.size(), ' ') << vChar;
            std::cout << "\033[" << (row + 2) << ";" << col << "H";
            std::cout << bl << repeatString(hChar, width) << br;
            }
        std::cout << getColorCode(RESET);
        }

    // ----- Same as Field but with Automatic width -----
    inline void Field(int row, int col, const std::string& msg,
        Color color = WHITE, bool all = false,
        BorderStyle style = EXTENDED) {
        int width = static_cast<int>(msg.size()) + 4;
        Field(row, col, width, msg, color, all, style);
        }

    }