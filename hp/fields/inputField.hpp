#pragma once

#include <iostream>
#include "hp/colors/color.hpp"
#include "hp/borderStyle/border.hpp"
#include "hp/string/string.hpp"
#include "hp/fields/field.hpp"
#include "hp/fields/centered.hpp"
#include <windows.h>
#include <conio.h>

namespace hp {

    // Input Field to get the input of the user | Use of ANSCII color from hp/colors/color.hpp | Use of ASCII Border style from hp/borderStyle/border.hpp
    inline std::string inputField(int row, int col, int width, const std::string placeholder = "", const std::string Allowed = "",
        int lenght = -1, hp::BorderStyle style = hp::EXTENDED) {
        if (lenght == -1) lenght = width; // The maximum lengh of the string
        BorderChars chars = hp::getBorderChars(hp::EXTENDED); // --> hp/borderStyle/border.hpp
        std::string input;
        std::string prevInput = " "; // To prevent redrawing each frame
        while (true) {
            if (input != prevInput) {
                std::cout << "\033[" << row << ";" << col << "H";
                std::cout << hp::getColorCode(hp::WHITE);
                std::cout << chars.tl << repeatString(chars.hChar, width) << chars.tr;
                std::cout << "\033[" << (row + 1) << ";" << col << "H";
                std::cout << chars.vChar;
                if (input.empty()) {
                    std::cout << hp::getColorCode(hp::BRIGHT_BLACK) << placeholder << hp::getColorCode(hp::WHITE);
                    std::cout << std::string(width - placeholder.size(), ' ');
                    }
                else {
                    std::cout << input;
                    std::cout << std::string(width - input.size(), ' ');
                    }
                std::cout << chars.vChar;
                std::cout << "\033[" << (row + 2) << ";" << col << "H";
                std::cout << chars.bl << repeatString(chars.hChar, width) << chars.br;
                std::cout << hp::getColorCode(hp::RESET);
                std::cout.flush();
                prevInput = input;
                }
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 13) return input;
                else if (ch == 8) { if (!input.empty()) input.pop_back(); }
                else if (ch == 27) return "";
                else if (ch >= 32 && ch <= 126 && input.size() < lenght) {
                    char c = (char)ch;
                    if (Allowed.empty() || Allowed.find(c) != std::string::npos) { // Prevent typing unwanted characters
                        input += c;
                        }
                    }
                }
            Sleep(10);
            }
        }

    // ----- Same Input Field as inputField but for password 
    inline std::string inputField(char secret, int row, int col, int width, const std::string placeholder = "", const std::string Allowed = "",
        int lenght = -1, hp::BorderStyle style = hp::EXTENDED) {
        if (lenght == -1) lenght = width; // The maximum lengh of the string
        BorderChars chars = hp::getBorderChars(hp::EXTENDED); // --> hp/borderStyle/border.hpp
        std::string input;
        std::string prevInput = " "; // To prevent redrawing each frame
        while (true) {
            if (input != prevInput) {
                std::cout << "\033[" << row << ";" << col << "H";
                std::cout << hp::getColorCode(hp::WHITE);
                std::cout << chars.tl << repeatString(chars.hChar, width) << chars.tr;
                std::cout << "\033[" << (row + 1) << ";" << col << "H";
                std::cout << chars.vChar;
                if (input.empty()) {
                    std::cout << hp::getColorCode(hp::BRIGHT_BLACK) << placeholder << hp::getColorCode(hp::WHITE);
                    std::cout << std::string(width - placeholder.size(), ' ');
                    }
                else {
                    std::cout << std::string((input.size()), secret); // Replace the input whit the specified character
                    std::cout << std::string(width - input.size(), ' ');
                    }
                std::cout << chars.vChar;
                std::cout << "\033[" << (row + 2) << ";" << col << "H";
                std::cout << chars.bl << repeatString(chars.hChar, width) << chars.br;
                std::cout << hp::getColorCode(hp::RESET);
                std::cout.flush();
                prevInput = input;
                }
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 13) return input;
                else if (ch == 8) { if (!input.empty()) input.pop_back(); }
                else if (ch == 27) return "";
                else if (ch >= 32 && ch <= 126 && input.size() < lenght) {
                    char c = (char)ch;
                    if (Allowed.empty() || Allowed.find(c) != std::string::npos) { // Prevent typing unwanted characters
                        input += c;
                        }
                    }
                }
            Sleep(10);
            }
        }

    // ----- Prevents characters other then : Intengers
    inline std::string inputInt(int row, int col, int width, const std::string& placeholder = "", hp::BorderStyle style = hp::EXTENDED) {
        return inputField(row, col, width, placeholder, "0123456789", -1, style);
        }

    // ----- Prevents characters other then : Floats
    inline std::string inputFloat(int row, int col, int width, const std::string& placeholder = "", hp::BorderStyle style = hp::EXTENDED) {
        return inputField(row, col, width, placeholder, "0123456789.", -1, style);
        }

    // ----- Prevents characters other then : Alpha Letters
    inline std::string inputAlpha(int row, int col, int width, const std::string& placeholder = "", hp::BorderStyle style = hp::EXTENDED) {
        return inputField(row, col, width, placeholder, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", -1, style);
        }

    // ----- Prevents characters other then : Alpha Characters
    inline std::string inputAlphaNum(int row, int col, int width, const std::string& placeholder = "", hp::BorderStyle style = hp::EXTENDED) {
        return inputField(row, col, width, placeholder, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", -1, style);
        }

    // ----- Functio to clear a previous Input field | Needs the exact same : row, col, width
    void clearInputField(int row, int col, int width) {
        for (int i = 0; i < 3; i++) {
            std::cout << "\033[" << (row + i) << ";" << col << "H";
            std::cout << "\033[2K";
            }
        }
    }
