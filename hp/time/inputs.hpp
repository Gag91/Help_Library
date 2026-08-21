#pragma once

#include <ctime>
#include <random>
#include <iostream>
#include "hp/fields/inputField.hpp"
#include "hp/keyboard/keyboard.hpp"
#include "hp/time/time.hpp"

namespace hp {

    // ----- Input Whit time limit
    inline std::string timedInput(int row, int col, int width, int timeLimit, const std::string& msg = "", const std::string time = "TIME'S UP!",
        const std::string allowed = "", bool showTime = false, BorderStyle style = EXTENDED) {
        std::string input;
        std::string prevInput = " ";
        auto timer = hp::startTimer();
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
            double elapsed = hp::stopTimer(timer);
            if (input != prevInput) {
                std::cout << "\033[" << row << ";" << col << "H";
                std::cout << getColorCode(WHITE);
                std::cout << tl << repeatString(hChar, width) << tr;
                std::cout << "\033[" << (row + 1) << ";" << col << "H";
                std::cout << vChar;
                if (elapsed >= timeLimit) {
                    std::cout << getColorCode(RED) << time << getColorCode(RESET);
                    std::cout << std::string(width - time.size(), ' ') << vChar;
                    hp::wait(0.5);
                    return "";
                    }
                else if (input.empty()) {
                    std::cout << getColorCode(BRIGHT_BLACK) << msg << getColorCode(WHITE);
                    std::cout << std::string(width - msg.size(), ' ');
                    }
                else {
                    std::cout << input;
                    std::cout << std::string(width - input.size(), ' ');
                    }
                std::cout << vChar;
                std::cout << "\033[" << (row + 2) << ";" << col << "H";
                std::cout << bl << repeatString(hChar, width) << br;
                std::cout << getColorCode(RESET);
                if (showTime) {
                    int remaining = timeLimit - (int)elapsed;
                    if (remaining < 0) remaining = 0;
                    std::cout << "\033[" << (row - 1) << ";" << col << "H\033[2K";
                    std::cout << "Time: " << remaining << "s";
                    }
                std::cout.flush();
                prevInput = input;
                }
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 13) return input;
                else if (ch == 8) { if (!input.empty()) input.pop_back(); }
                else if (ch == 27) return "";
                else if (ch >= 32 && ch <= 126 && input.size() < width) {
                    char c = (char)ch;
                    if (allowed.empty() || allowed.find(c) != std::string::npos) {
                        input += c;
                        }
                    }
                }
            Sleep(10);
            }
        }

    // ----- Centered Field that auto delete after certain amount of time set
    inline void centeredNotify(int row, const std::string& msg, hp::Color color = hp::GREEN, double duration = 1.5, BorderStyle style = EXTENDED) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int boxWidth = msg.length() + 4;
        int col = (consoleWidth - boxWidth) / 2;
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
        std::cout << tl << repeatString(hChar, boxWidth) << tr;
        std::cout << "\033[" << (row + 1) << ";" << col << "H";
        std::cout << vChar << " " << getColorCode(color) << msg << getColorCode(RESET) << std::string(((boxWidth - msg.size()) - 1), ' ') << vChar;
        std::cout << "\033[" << (row + 2) << ";" << col << "H";
        std::cout << bl << repeatString(hChar, boxWidth) << br;
        hp::wait(duration);
        clearInputField(row, col, boxWidth);
        }

    }