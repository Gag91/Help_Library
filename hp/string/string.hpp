#pragma once
#include <iostream>
#include <functional>
#include "hp/string/inputs.hpp"

namespace hp {

    // ----- Regular title function
    inline void title(const std::string& t, int w = 40) {
        std::string sep = std::string(w, '=');
        std::cout << sep << std::endl;
        std::cout << t << std::endl;
        std::cout << sep << std::endl;
        }
    // ----- Works the same as "std::string(int, char)" but maded to use string instead
    std::string repeatString(const std::string& str, int count) {
        std::string result;
        for (int i = 0; i < count; i++) {
            result += str;
            }
        return result;
        }

    // ----- Centered Seperator
    inline void sp(const std::string& msg, char a = '=') {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int msgLen = msg.length() + 2;
        int totalWidth = consoleWidth;
        int sideLen = (totalWidth - msgLen) / 2;

        std::string result;
        result.reserve(totalWidth);
        result.append(sideLen, a);
        result += " " + msg + " ";
        result.append(sideLen, a);
        if (result.length() < totalWidth) {
            result += a;
            }
        std::cout << "\033[2K\r";
        std::cout << result << '\n';
        }
    // ----- Wait for enter key to be pressed
    inline void waitForEnter(std::function<void()> func = nullptr) {
        std::cout << "Press Enter To Continue..." << std::endl;
        std::cout.flush();
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        if (func) {
            func();
            }
        }

    bool starts_with(const std::string& str, const std::string& prefix) {
        return str.starts_with(prefix);
        }

    bool ends_with(const std::string& str, const std::string& suffix) {
        return str.ends_with(suffix);
        }

    }