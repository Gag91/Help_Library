#pragma once

#include <iostream>
#include <string>

namespace hp {
    enum BorderStyle {
        ASCII,
        EXTENDED,
        DOUBLE
        };

    struct BorderChars {
        std::string tl, tr, bl, br, sep, hChar, vChar;
        };

    inline BorderChars getBorderChars(BorderStyle style) {
        BorderChars chars;

        switch (style) {
            case hp::ASCII:
                chars.tl = "+"; chars.tr = "+";
                chars.bl = "+"; chars.br = "+";
                chars.sep = "+"; chars.hChar = "-"; chars.vChar = "|";
                break;
            case hp::EXTENDED:
                chars.tl = "┌"; chars.tr = "┐";
                chars.bl = "└"; chars.br = "┘";
                chars.sep = "├"; chars.hChar = "─"; chars.vChar = "│";
                break;
            case hp::DOUBLE:
                chars.tl = "╔"; chars.tr = "╗";
                chars.bl = "╚"; chars.br = "╝";
                chars.sep = "╠"; chars.hChar = "═"; chars.vChar = "║";
                break;
            default:
                throw std::runtime_error("Invalid BorderStyle!");
            }

        return chars;
        }
    }