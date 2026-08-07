#pragma once

#include <windows.h>
#include <conio.h>
#include <iostream>

namespace hp {

    // ----- Released namespace
    namespace KeyIsReleased {
        inline bool A = false, B = false, C = false, D = false, E = false;
        inline bool F = false, G = false, H = false, I = false, J = false;
        inline bool K = false, L = false, M = false, N = false, O = false;
        inline bool P = false, Q = false, R = false, S = false, T = false;
        inline bool U = false, V = false, W = false, X = false, Y = false;
        inline bool Z = false;
        inline bool Up = false, Down = false, Left = false, Right = false;
        inline bool Escape = false, Enter = false, Space = false;
        inline bool Shift = false, Control = false, Alt = false;
        inline bool L_Mouse = false, R_Mouse = false;
        }
    }