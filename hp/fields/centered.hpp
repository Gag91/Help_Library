#pragma once

#include "hp/borderStyle/border.hpp"
#include "hp/colors/color.hpp"
#include "hp/fields/field.hpp"
#include "hp/keyboard/keyboard.hpp"
#include "hp/platform/console.hpp"
#include "hp/string/string.hpp"
#include "hp/time/time.hpp"

#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace hp {

    inline void centeredBox(int row, int width, const std::string &msg, hp::Color color = hp::WHITE, BorderStyle style = EXTENDED) {
        int consoleWidth = hp::platform::console_width();
        if (width == -1) {
            width = static_cast<int>(msg.size()) + 4;
        }
        int col = (consoleWidth - width) / 2;
        BorderChars chars = hp::getBorderChars(style);
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << chars.tl << repeatString(chars.hChar, width) << chars.tr;
        std::cout << "\033[" << (row + 1) << ";" << col << "H";
        std::cout << chars.vChar << getColorCode(color) << msg << getColorCode(RESET) << std::string(width - msg.size(), ' ') << chars.vChar;
        std::cout << "\033[" << (row + 2) << ";" << col << "H";
        std::cout << chars.bl << repeatString(chars.hChar, width) << chars.br;
    }

    template <typename T = std::string>
    std::optional<T> CenteredInput(int row, int width, const std::string &msg, std::string allowed = "", hp::Color color = hp::WHITE, int length = -1, BorderStyle style = EXTENDED) {
        if (length == -1)
            length = width;
        int consoleWidth = hp::platform::console_width();
        if (width < static_cast<int>(msg.length()) + 2) {
            width = static_cast<int>(msg.length()) + 4;
        }
        int col = (consoleWidth - width) / 2;
        std::string input;
        std::string prevInput = " ";
        BorderChars chars = hp::getBorderChars(style);
        while (true) {
            if (input != prevInput) {
                std::cout << "\033[" << row << ";" << col << "H";
                std::cout << getColorCode(hp::WHITE);
                std::cout << chars.tl << repeatString(chars.hChar, width) << chars.tr;
                std::cout << "\033[" << (row + 1) << ";" << col << "H";
                std::cout << chars.vChar;
                if (input.empty()) {
                    std::cout << getColorCode(hp::BRIGHT_BLACK) << msg << getColorCode(hp::WHITE);
                    std::cout << std::string(width - msg.size(), ' ');
                } else {
                    std::cout << getColorCode(color) << input << getColorCode(hp::WHITE);
                    std::cout << std::string(width - input.size(), ' ');
                }
                std::cout << chars.vChar;
                std::cout << "\033[" << (row + 2) << ";" << col << "H";
                std::cout << chars.bl << repeatString(chars.hChar, width) << chars.br;
                std::cout << getColorCode(hp::RESET);
                std::cout.flush();
                prevInput = input;
            }
            if (hp::platform::kbhit()) {
                int ch = hp::platform::getch();
                if (ch == 13 || ch == 10) {
                    hp::platform::disable_raw();
                    if (input.empty())
                        return std::nullopt;
                    std::stringstream ss(input);
                    T result;
                    if (ss >> result)
                        return result;
                    return std::nullopt;
                } else if (ch == 8 || ch == 127) {
                    if (!input.empty())
                        input.pop_back();
                } else if (ch == 27) {
                    hp::platform::disable_raw();
                    return std::nullopt;
                } else if (ch >= 32 && ch <= 126 && static_cast<int>(input.size()) < length) {
                    char c = static_cast<char>(ch);
                    if (allowed.empty() || allowed.find(c) != std::string::npos) {
                        input += c;
                    }
                }
            }
            hp::platform::sleep_ms(10);
        }
    }

    inline int CenteredFieldMenu(int startRow, int width, const std::vector<std::string> options, hp::Color color = hp::WHITE, BorderStyle style = EXTENDED) {
        hp::platform::flush_input();
        int consoleWidth = hp::platform::console_width();
        int col = (consoleWidth - width) / 2;
        if (col < 1)
            col = 1;
        int index = 0;
        int prevIndex = -1;
        int maxIndex = static_cast<int>(options.size()) - 1;
        for (int i = 0; i <= maxIndex; i++) {
            int row = startRow + (i * 3);
            hp::Field(row, col, width, options[i], hp::WHITE, true, style);
        }
        while (true) {
            hp::updateKeyboard();
            if (hp::KeyIsPressed::Up) {
                index = (index > 0) ? index - 1 : maxIndex;
            } else if (hp::KeyIsPressed::Down) {
                index = (index < maxIndex) ? index + 1 : 0;
            } else if (hp::KeyIsPressed::Enter) {
                hp::platform::disable_raw();
                return index;
            } else if (hp::KeyIsPressed::Escape) {
                hp::platform::disable_raw();
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
            hp::platform::sleep_ms(50);
        }
    }

    inline void clearCenteredInput(int row, int width) {
        int consoleWidth = hp::platform::console_width();
        int col = (consoleWidth - width) / 2;
        for (int i = 0; i < 3; i++) {
            std::cout << "\033[" << (row + i) << ";" << col << "H";
            std::cout << "\033[2K";
        }
    }
} // namespace hp