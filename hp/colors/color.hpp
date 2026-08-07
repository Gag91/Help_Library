#pragma once
#include <iostream>

namespace hp {

    enum Color {
        BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,
        BRIGHT_BLACK, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW,
        BRIGHT_BLUE, BRIGHT_MAGENTA, BRIGHT_CYAN, BRIGHT_WHITE,
        BG_BLACK, BG_RED, BG_GREEN, BG_YELLOW, BG_BLUE, BG_MAGENTA, BG_CYAN, BG_WHITE,
        RESET, COLOR_COUNT
        };

    // ----- Get Color
    inline std::string getColorCode(Color color) {
        switch (color) {
            case BLACK:          return "\033[30m";
            case RED:            return "\033[31m";
            case GREEN:          return "\033[32m";
            case YELLOW:         return "\033[33m";
            case BLUE:           return "\033[34m";
            case MAGENTA:        return "\033[35m";
            case CYAN:           return "\033[36m";
            case WHITE:          return "\033[37m";
            case BRIGHT_BLACK:   return "\033[90m";
            case BRIGHT_RED:     return "\033[91m";
            case BRIGHT_GREEN:   return "\033[92m";
            case BRIGHT_YELLOW:  return "\033[93m";
            case BRIGHT_BLUE:    return "\033[94m";
            case BRIGHT_MAGENTA: return "\033[95m";
            case BRIGHT_CYAN:    return "\033[96m";
            case BRIGHT_WHITE:   return "\033[97m";
            case BG_RED:         return "\033[41m";
            case BG_GREEN:       return "\033[42m";
            case BG_BLUE:        return "\033[44m";
            case RESET:          return "\033[0m";
            default:             return "";
            }
        }

    // ----- Print whit colors
    template<typename... Args>
    inline void printCl(Color color, Args... args) {
        std::cout << getColorCode(color);
        (std::cout << ... << args);
        std::cout << "\033[0m";
        }

    // ----- PrintLn whit colors
    template<typename... Args>
    inline void printlnCl(Color color, Args...args) {
        std::cout << getColorCode(color);
        (std::cout << ... << args);
        std::cout << "\033[0m\n";
        }
    }