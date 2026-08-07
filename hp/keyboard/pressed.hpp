#pragma once

#include <windows.h>
#include <conio.h>
#include <iostream>

namespace hp {

    // ----- Pressed namespace
    namespace KeyIsPressed {
        inline bool A = false, B = false, C = false, D = false, E = false;
        inline bool F = false, G = false, H = false, I = false, J = false;
        inline bool K = false, L = false, M = false, N = false, O = false;
        inline bool P = false, Q = false, R = false, S = false, T = false;
        inline bool U = false, V = false, W = false, X = false, Y = false;
        inline bool Z = false;
        inline bool Num0 = false, Num1 = false, Num2 = false, Num3 = false;
        inline bool Num4 = false, Num5 = false, Num6 = false, Num7 = false;
        inline bool Num8 = false, Num9 = false;
        inline bool Up = false, Down = false, Left = false, Right = false;
        inline bool Escape = false, Enter = false, Space = false;
        inline bool Tab = false, Backspace = false, Delete = false;
        inline bool Insert = false, Home = false, End = false;
        inline bool PageUp = false, PageDown = false;
        inline bool Shift = false, Control = false, Alt = false;
        inline bool F1 = false, F2 = false, F3 = false, F4 = false;
        inline bool F5 = false, F6 = false, F7 = false, F8 = false;
        inline bool F9 = false, F10 = false, F11 = false, F12 = false;
        inline bool L_Mouse = false, R_Mouse = false;
        }
    }