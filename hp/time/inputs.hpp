#pragma once

#include "hp/borderStyle/border.hpp"
#include "hp/colors/color.hpp"
#include "hp/fields/inputField.hpp"
#include "hp/platform/console.hpp"
#include "hp/string/string.hpp"
#include "hp/time/time.hpp"

#include <iostream>
#include <string>

namespace hp {

    inline std::string timedInput(int row, int col, int width, int timeLimit, const std::string &msg = "", const std::string &time = "TIME'S UP!", const std::string &allowed = "", bool showTime = false, BorderStyle style = EXTENDED) {
        std::string input;
        std::string prevInput = " ";
        auto timer = hp::startTimer();
        BorderChars chars = hp::getBorderChars(style);
        while (true) {
            double elapsed = hp::stopTimer(timer);
            if (input != prevInput) {
                std::cout << "\033[" << row << ";" << col << "H";
                std::cout << getColorCode(WHITE);
                std::cout << chars.tl << repeatString(chars.hChar, width) << chars.tr;
                std::cout << "\033[" << (row + 1) << ";" << col << "H";
                std::cout << chars.vChar;
                if (elapsed >= timeLimit) {
                    std::cout << getColorCode(RED) << time << getColorCode(RESET);
                    std::cout << std::string(width - time.size(), ' ') << chars.vChar;
                    hp::wait(0.5);
                    hp::platform::disable_raw();
                    return "";
                } else if (input.empty()) {
                    std::cout << getColorCode(BRIGHT_BLACK) << msg << getColorCode(WHITE);
                    std::cout << std::string(width - msg.size(), ' ');
                } else {
                    std::cout << input;
                    std::cout << std::string(width - input.size(), ' ');
                }
                std::cout << chars.vChar;
                std::cout << "\033[" << (row + 2) << ";" << col << "H";
                std::cout << chars.bl << repeatString(chars.hChar, width) << chars.br;
                std::cout << getColorCode(RESET);
                if (showTime) {
                    int remaining = timeLimit - static_cast<int>(elapsed);
                    if (remaining < 0)
                        remaining = 0;
                    std::cout << "\033[" << (row - 1) << ";" << col << "H\033[2K";
                    std::cout << "Time: " << remaining << "s";
                }
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
                } else if (ch >= 32 && ch <= 126 && static_cast<int>(input.size()) < width) {
                    char c = static_cast<char>(ch);
                    if (allowed.empty() || allowed.find(c) != std::string::npos) {
                        input += c;
                    }
                }
            }
            hp::platform::sleep_ms(10);
        }
    }

    inline void centeredNotify(int row, const std::string &msg, hp::Color color = hp::GREEN, double duration = 1.5, BorderStyle style = EXTENDED) {
        int consoleWidth = hp::platform::console_width();
        int boxWidth = static_cast<int>(msg.length()) + 4;
        int col = (consoleWidth - boxWidth) / 2;
        BorderChars chars = hp::getBorderChars(style);
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout << chars.tl << repeatString(chars.hChar, boxWidth) << chars.tr;
        std::cout << "\033[" << (row + 1) << ";" << col << "H";
        std::cout << chars.vChar << " " << getColorCode(color) << msg << getColorCode(RESET) << std::string(boxWidth - msg.size() - 1, ' ') << chars.vChar;
        std::cout << "\033[" << (row + 2) << ";" << col << "H";
        std::cout << chars.bl << repeatString(chars.hChar, boxWidth) << chars.br;
        hp::wait(duration);
        clearInputField(row, col, boxWidth);
    }
} // namespace hp