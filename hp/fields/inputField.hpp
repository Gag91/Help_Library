#pragma once

#include "hp/borderStyle/border.hpp"
#include "hp/colors/color.hpp"
#include "hp/fields/centered.hpp"
#include "hp/fields/field.hpp"
#include "hp/platform/console.hpp"
#include "hp/string/string.hpp"

#include <iostream>
#include <string>

namespace hp {

    inline std::string inputField(int row, int col, int width, const std::string &placeholder = "", const std::string &Allowed = "", int length = -1, hp::BorderStyle style = hp::EXTENDED) {
        if (length == -1)
            length = width;
        BorderChars chars = hp::getBorderChars(style);
        std::string input;
        std::string prevInput = " ";
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
                } else {
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
            if (hp::platform::kbhit()) {
                int ch = hp::platform::getch();
                if (ch == 13 || ch == 10) {
                    hp::platform::disable_raw();
                    return input;
                } else if (ch == 8 || ch == 127) {
                    if (!input.empty())
                        input.pop_back();
                } else if (ch == 27) {
                    hp::platform::disable_raw();
                    return "";
                } else if (ch >= 32 && ch <= 126 && static_cast<int>(input.size()) < length) {
                    char c = static_cast<char>(ch);
                    if (Allowed.empty() || Allowed.find(c) != std::string::npos) {
                        input += c;
                    }
                }
            }
            hp::platform::sleep_ms(10);
        }
    }

    inline std::string inputField(char secret, int row, int col, int width, const std::string &placeholder = "", const std::string &Allowed = "", int length = -1, hp::BorderStyle style = hp::EXTENDED) {
        if (length == -1)
            length = width;
        BorderChars chars = hp::getBorderChars(style);
        std::string input;
        std::string prevInput = " ";
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
                } else {
                    std::cout << std::string(input.size(), secret);
                    std::cout << std::string(width - input.size(), ' ');
                }
                std::cout << chars.vChar;
                std::cout << "\033[" << (row + 2) << ";" << col << "H";
                std::cout << chars.bl << repeatString(chars.hChar, width) << chars.br;
                std::cout << hp::getColorCode(hp::RESET);
                std::cout.flush();
                prevInput = input;
            }
            if (hp::platform::kbhit()) {
                int ch = hp::platform::getch();
                if (ch == 13 || ch == 10) {
                    hp::platform::disable_raw();
                    return input;
                } else if (ch == 8 || ch == 127) {
                    if (!input.empty())
                        input.pop_back();
                } else if (ch == 27) {
                    hp::platform::disable_raw();
                    return "";
                } else if (ch >= 32 && ch <= 126 && static_cast<int>(input.size()) < length) {
                    char c = static_cast<char>(ch);
                    if (Allowed.empty() || Allowed.find(c) != std::string::npos) {
                        input += c;
                    }
                }
            }
            hp::platform::sleep_ms(10);
        }
    }

    inline std::string inputInt(int row, int col, int width, const std::string &placeholder = "", hp::BorderStyle style = hp::EXTENDED) {
        return inputField(row, col, width, placeholder, "0123456789-", -1, style);
    }

    inline std::string inputFloat(int row, int col, int width, const std::string &placeholder = "", hp::BorderStyle style = hp::EXTENDED) {
        return inputField(row, col, width, placeholder, "0123456789.-", -1, style);
    }

    inline std::string inputAlpha(int row, int col, int width, const std::string &placeholder = "", hp::BorderStyle style = hp::EXTENDED) {
        return inputField(row, col, width, placeholder, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", -1, style);
    }

    inline std::string inputAlphaNum(int row, int col, int width, const std::string &placeholder = "", hp::BorderStyle style = hp::EXTENDED) {
        return inputField(row, col, width, placeholder, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", -1, style);
    }

    inline void clearInputField(int row, int col, int width) {
        (void)width;
        for (int i = 0; i < 3; i++) {
            std::cout << "\033[" << (row + i) << ";" << col << "H";
            std::cout << "\033[2K";
        }
    }
} // namespace hp