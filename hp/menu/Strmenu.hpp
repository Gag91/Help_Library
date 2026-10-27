#pragma once

#include "hp/colors/color.hpp"
#include "hp/keyboard/keyboard.hpp"
#include "hp/platform/console.hpp"
#include "hp/string/string.hpp"
#include "hp/time/time.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace hp {

    inline void setMenu(const std::string &t, const std::vector<std::string> &o, bool showNumbers = true) {
        hp::title(t);
        for (size_t i = 0; i < o.size(); i++) {
            if (showNumbers) {
                std::cout << (i + 1) << o[i] << std::endl;
            } else {
                std::cout << o[i] << std::endl;
            }
        }
        std::cout << "" << std::endl;
    }

    inline int arrowMenu(const std::string &t, const std::vector<std::string> &o, bool showNumbers = true) {
        hp::platform::flush_input();
        int index = 0;
        int maxindex = static_cast<int>(o.size()) - 1;
        int prevIndex = -1;
        std::string sep = std::string(40, '=');
        std::cout << sep << '\n'
                  << t << '\n'
                  << sep << '\n';
        for (int i = 0; i <= maxindex; i++) {
            if (showNumbers) {
                std::cout << "  " << (i + 1) << ". " << o[i] << '\n';
            } else {
                std::cout << "  " << o[i] << '\n';
            }
        }
        while (true) {
            if (index != prevIndex) {
                std::cout << "\033[" << (maxindex + 1) << "A";
                for (int i = 0; i <= maxindex; i++) {
                    std::cout << "\033[2K";
                    if (i == index) {
                        std::cout << "> " << (i + 1) << ". " << o[i] << " <\n";
                    } else {
                        std::cout << "  " << (i + 1) << ". " << o[i] << '\n';
                    }
                }
                std::cout.flush();
                prevIndex = index;
            }
            hp::updateKeyboard();
            if (hp::KeyIsPressed::Up) {
                index = (index > 0) ? index - 1 : maxindex;
            } else if (hp::KeyIsPressed::Down) {
                index = (index < maxindex) ? index + 1 : 0;
            } else if (hp::KeyIsPressed::Enter) {
                hp::platform::disable_raw();
                std::cout << '\n';
                return index;
            }
            hp::wait(0.05);
        }
    }

    inline int CenteredMenu(const std::vector<std::string> &o, Color color = WHITE, bool showNumbers = true) {
        hp::platform::flush_input();
        int index = 0;
        int maxindex = static_cast<int>(o.size()) - 1;
        int prevIndex = -1;
        int consoleWidth = hp::platform::console_width();
        int maxOptionWidth = 0;
        for (auto &opt : o) {
            int w = static_cast<int>(opt.size()) + 6;
            if (w > maxOptionWidth)
                maxOptionWidth = w;
        }
        int totalWidth = maxOptionWidth;
        if (totalWidth < 20)
            totalWidth = 20;
        int col = (consoleWidth - totalWidth) / 2;
        if (col < 1)
            col = 1;
        int startRow = 3;
        for (int i = 0; i <= maxindex; i++) {
            std::cout << "\033[" << (startRow + i) << ";" << col << "H";
            if (showNumbers) {
                std::cout << "  " << (i + 1) << ". " << o[i];
            } else {
                std::cout << "  " << o[i];
            }
        }
        while (true) {
            if (index != prevIndex) {
                for (int i = 0; i <= maxindex; i++) {
                    std::cout << "\033[" << (startRow + i) << ";" << col << "H\033[2K";
                    if (i == index) {
                        std::cout << getColorCode(color) << "> " << (i + 1) << ". " << o[i] << " <" << getColorCode(RESET);
                    } else {
                        std::cout << "  " << (i + 1) << ". " << o[i];
                    }
                }
                std::cout.flush();
                prevIndex = index;
            }
            hp::updateKeyboard();
            if (hp::KeyIsPressed::Up) {
                index = (index > 0) ? index - 1 : maxindex;
            } else if (hp::KeyIsPressed::Down) {
                index = (index < maxindex) ? index + 1 : 0;
            } else if (hp::KeyIsPressed::Enter) {
                hp::platform::disable_raw();
                std::cout << '\n';
                return index;
            }
            hp::wait(0.05);
        }
    }
} // namespace hp