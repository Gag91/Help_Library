#pragma once

#include <iostream>
#include "hp/colors/color.hpp"
#include "hp/borderStyle/border.hpp"
#include "hp/keyboard/keyboard.hpp"
#include "hp/time/time.hpp"
#include "hp/menu/Strmenu.hpp"
#include <limits>
#include <vector>

namespace hp {

    // ----- Centered Field Menu navigateable whit arrow keys
    inline int CenteredFieldMenu(const std::string& t, const std::vector<std::string>& o, hp::Color color = hp::WHITE, bool showNumbers = true) {
        while (_kbhit()) _getch();
        int index = 0;
        int maxindex = static_cast<int>(o.size()) - 1;
        int prevIndex = -1;

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

        int maxOptionWidth = 0;
        for (auto& opt : o) {
            int w = opt.size() + 6;
            if (w > maxOptionWidth) maxOptionWidth = w;
            }

        int totalWidth = maxOptionWidth;
        if (totalWidth < 20) totalWidth = 20;
        int col = (consoleWidth - totalWidth) / 2;
        if (col < 1) col = 1;

        std::string sep = std::string(totalWidth, '=');
        int titlePadding = (totalWidth - t.size()) / 2;
        std::cout << "\033[3;" << col << "H" << sep;
        std::cout << "\033[4;" << col << "H" << std::string(titlePadding, ' ') << t;
        std::cout << "\033[5;" << col << "H" << sep;
        for (int i = 0; i <= maxindex; i++) {
            std::cout << "\033[" << (6 + i) << ";" << col << "H";
            std::cout << "  " << (i + 1) << ". " << o[i];
            }

        while (true) {
            if (index != prevIndex) {
                for (int i = 0; i <= maxindex; i++) {
                    std::cout << "\033[" << (6 + i) << ";" << col << "H\033[2K";
                    if (i == index) {
                        std::cout << hp::getColorCode(color) << "> " << (i + 1) << ". " << o[i] << " <" << hp::getColorCode(hp::RESET);
                        }
                    else {
                        std::cout << "  " << (i + 1) << ". " << o[i];
                        }
                    }

                std::cout.flush();
                prevIndex = index;
                }
            hp::updateKeyboard();
            if (hp::KeyIsPressed::Up) {
                index = (index > 0) ? index - 1 : maxindex;
                }
            else if (hp::KeyIsPressed::Down) {
                index = (index < maxindex) ? index + 1 : 0;
                }
            else if (hp::KeyIsPressed::Enter) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << '\n';
                return index;
                }
            hp::wait(0.05);
            }
        }
    }