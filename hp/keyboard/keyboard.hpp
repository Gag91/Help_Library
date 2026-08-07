#pragma once

#include <windows.h>
#include <conio.h>
#include <map>
#include <unordered_map>
#include <iostream>
#include "hp/keyboard/pressed.hpp"
#include "hp/keyboard/hold.hpp"
#include "hp/keyboard/released.hpp"
#include "hp/keyboard/repeat.hpp"

// ----- Defines 
#ifndef VK_Space
#define VK_Space VK_SPACE
#endif
#ifndef VK_Enter
#define VK_Enter VK_RETURN
#endif
#ifndef VK_Tab
#define VK_Tab VK_TAB
#endif
#ifndef VK_Backspace
#define VK_Backspace VK_BACK
#endif
#ifndef VK_Delete
#define VK_Delete VK_DELETE
#endif
#ifndef VK_Escape
#define VK_Escape VK_ESCAPE
#endif
#ifndef VK_Shift
#define VK_Shift VK_SHIFT
#endif
#ifndef VK_Control
#define VK_Control VK_CONTROL
#endif
#ifndef VK_Alt
#define VK_Alt VK_MENU
#endif
#ifndef VK_Up
#define VK_Up VK_UP
#endif
#ifndef VK_Down
#define VK_Down VK_DOWN
#endif
#ifndef VK_Left
#define VK_Left VK_LEFT
#endif
#ifndef VK_Right
#define VK_Right VK_RIGHT
#endif
#ifndef VK_Insert
#define VK_Insert VK_INSERT
#endif
#ifndef VK_Home
#define VK_Home VK_HOME
#endif
#ifndef VK_End
#define VK_End VK_END
#endif
#ifndef VK_PageUp
#define VK_PageUp VK_PRIOR
#endif
#ifndef VK_PageDown
#define VK_PageDown VK_NEXT
#endif
#ifndef VK_L_Mouse
#define VK_L_Mouse VK_LBUTTON
#endif
#ifndef VK_R_Mouse
#define VK_R_Mouse VK_RBUTTON
#endif

namespace hp {

    // ----- Keys
    enum class Key {
        A = 'A', B = 'B', C = 'C', D = 'D', E = 'E',
        F = 'F', G = 'G', H = 'H', I = 'I', J = 'J',
        K = 'K', L = 'L', M = 'M', N = 'N', O = 'O',
        P = 'P', Q = 'Q', R = 'R', S = 'S', T = 'T',
        U = 'U', V = 'V', W = 'W', X = 'X', Y = 'Y',
        Z = 'Z',
        Num0 = '0', Num1 = '1', Num2 = '2', Num3 = '3',
        Num4 = '4', Num5 = '5', Num6 = '6', Num7 = '7',
        Num8 = '8', Num9 = '9',
        Escape = VK_ESCAPE, Enter = VK_RETURN, Space = VK_SPACE,
        Tab = VK_TAB, Backspace = VK_BACK, Delete = VK_DELETE,
        Insert = VK_INSERT, Home = VK_HOME, End = VK_END,
        PageUp = VK_PRIOR, PageDown = VK_NEXT,
        Up = VK_UP, Down = VK_DOWN, Left = VK_LEFT, Right = VK_RIGHT,
        Shift = VK_SHIFT, Control = VK_CONTROL, Alt = VK_MENU,
        F1 = VK_F1, F2 = VK_F2, F3 = VK_F3, F4 = VK_F4,
        F5 = VK_F5, F6 = VK_F6, F7 = VK_F7, F8 = VK_F8,
        F9 = VK_F9, F10 = VK_F10, F11 = VK_F11, F12 = VK_F12
        };

    // ----- Main Class
    class Keyboard {
    private:
        static std::unordered_map<int, bool> previousStates;
        static std::unordered_map<int, bool> currentStates;

    public:

        // Update the keys
        static void update() {
            for (int i = 0; i < 256; i++) {
                previousStates[i] = currentStates[i];
                currentStates[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
                }
            }

        // Check if 'key' is pressed
        static bool isPressed(int key) {
            return currentStates[key] && !previousStates[key];
            }

        // Check if 'key' is hold
        static bool isHold(int key) {
            return currentStates[key];
            }

        // Check if 'key' is released
        static bool isReleased(int key) {
            return !currentStates[key] && previousStates[key];
            }

        // Check if 'key' is down
        static bool isDown(int key) {
            return isHold(key) || isPressed(key);
            }
        };

    inline std::unordered_map<int, bool> Keyboard::previousStates;
    inline std::unordered_map<int, bool> Keyboard::currentStates;

    // ----------- MOST IMPORTANT FUNCTION !!!! This function is required to make all other keyboard functions WORKS!!!! ---------- //
    inline void updateKeyboard() {
        Keyboard::update();

        // ----- Updates Pressed Function
#define UPDATE_PRESSED(key) KeyIsPressed::key = Keyboard::isPressed(VK_##key)
#define UPDATE_PRESSED_LETTER(key) KeyIsPressed::key = Keyboard::isPressed(#key[0])

        UPDATE_PRESSED_LETTER(A); UPDATE_PRESSED_LETTER(B);
        UPDATE_PRESSED_LETTER(C); UPDATE_PRESSED_LETTER(D);
        UPDATE_PRESSED_LETTER(E); UPDATE_PRESSED_LETTER(F);
        UPDATE_PRESSED_LETTER(G); UPDATE_PRESSED_LETTER(H);
        UPDATE_PRESSED_LETTER(I); UPDATE_PRESSED_LETTER(J);
        UPDATE_PRESSED_LETTER(K); UPDATE_PRESSED_LETTER(L);
        UPDATE_PRESSED_LETTER(M); UPDATE_PRESSED_LETTER(N);
        UPDATE_PRESSED_LETTER(O); UPDATE_PRESSED_LETTER(P);
        UPDATE_PRESSED_LETTER(Q); UPDATE_PRESSED_LETTER(R);
        UPDATE_PRESSED_LETTER(S); UPDATE_PRESSED_LETTER(T);
        UPDATE_PRESSED_LETTER(U); UPDATE_PRESSED_LETTER(V);
        UPDATE_PRESSED_LETTER(W); UPDATE_PRESSED_LETTER(X);
        UPDATE_PRESSED_LETTER(Y); UPDATE_PRESSED_LETTER(Z);

        KeyIsPressed::Num0 = Keyboard::isPressed('0');
        KeyIsPressed::Num1 = Keyboard::isPressed('1');
        KeyIsPressed::Num2 = Keyboard::isPressed('2');
        KeyIsPressed::Num3 = Keyboard::isPressed('3');
        KeyIsPressed::Num4 = Keyboard::isPressed('4');
        KeyIsPressed::Num5 = Keyboard::isPressed('5');
        KeyIsPressed::Num6 = Keyboard::isPressed('6');
        KeyIsPressed::Num7 = Keyboard::isPressed('7');
        KeyIsPressed::Num8 = Keyboard::isPressed('8');
        KeyIsPressed::Num9 = Keyboard::isPressed('9');

        UPDATE_PRESSED(Up); UPDATE_PRESSED(Down);
        UPDATE_PRESSED(Left); UPDATE_PRESSED(Right);
        UPDATE_PRESSED(Escape); UPDATE_PRESSED(Enter);
        UPDATE_PRESSED(Space); UPDATE_PRESSED(Tab);
        UPDATE_PRESSED(Backspace); UPDATE_PRESSED(Delete);
        UPDATE_PRESSED(Insert); UPDATE_PRESSED(Home);
        UPDATE_PRESSED(End); UPDATE_PRESSED(PageUp);
        UPDATE_PRESSED(PageDown);
        UPDATE_PRESSED(Shift); UPDATE_PRESSED(Control);
        UPDATE_PRESSED(Alt); UPDATE_PRESSED(L_Mouse);
        UPDATE_PRESSED(R_Mouse);

        UPDATE_PRESSED(F1); UPDATE_PRESSED(F2);
        UPDATE_PRESSED(F3); UPDATE_PRESSED(F4);
        UPDATE_PRESSED(F5); UPDATE_PRESSED(F6);
        UPDATE_PRESSED(F7); UPDATE_PRESSED(F8);
        UPDATE_PRESSED(F9); UPDATE_PRESSED(F10);
        UPDATE_PRESSED(F11); UPDATE_PRESSED(F12);

        // ----- Updates Hold keys Function
#define UPDATE_HOLD(key) KeyIsHold::key = Keyboard::isHold(VK_##key)
#define UPDATE_HOLD_LETTER(key) KeyIsHold::key = Keyboard::isHold(#key[0])

        UPDATE_HOLD_LETTER(A); UPDATE_HOLD_LETTER(B);
        UPDATE_HOLD_LETTER(C); UPDATE_HOLD_LETTER(D);
        UPDATE_HOLD_LETTER(E); UPDATE_HOLD_LETTER(F);
        UPDATE_HOLD_LETTER(G); UPDATE_HOLD_LETTER(H);
        UPDATE_HOLD_LETTER(I); UPDATE_HOLD_LETTER(J);
        UPDATE_HOLD_LETTER(K); UPDATE_HOLD_LETTER(L);
        UPDATE_HOLD_LETTER(M); UPDATE_HOLD_LETTER(N);
        UPDATE_HOLD_LETTER(O); UPDATE_HOLD_LETTER(P);
        UPDATE_HOLD_LETTER(Q); UPDATE_HOLD_LETTER(R);
        UPDATE_HOLD_LETTER(S); UPDATE_HOLD_LETTER(T);
        UPDATE_HOLD_LETTER(U); UPDATE_HOLD_LETTER(V);
        UPDATE_HOLD_LETTER(W); UPDATE_HOLD_LETTER(X);
        UPDATE_HOLD_LETTER(Y); UPDATE_HOLD_LETTER(Z);

        KeyIsHold::Num0 = Keyboard::isHold('0');
        KeyIsHold::Num1 = Keyboard::isHold('1');
        KeyIsHold::Num2 = Keyboard::isHold('2');
        KeyIsHold::Num3 = Keyboard::isHold('3');
        KeyIsHold::Num4 = Keyboard::isHold('4');
        KeyIsHold::Num5 = Keyboard::isHold('5');
        KeyIsHold::Num6 = Keyboard::isHold('6');
        KeyIsHold::Num7 = Keyboard::isHold('7');
        KeyIsHold::Num8 = Keyboard::isHold('8');
        KeyIsHold::Num9 = Keyboard::isHold('9');

        UPDATE_HOLD(Up); UPDATE_HOLD(Down);
        UPDATE_HOLD(Left); UPDATE_HOLD(Right);
        UPDATE_HOLD(Escape); UPDATE_HOLD(Enter);
        UPDATE_HOLD(Space); UPDATE_HOLD(Tab);
        UPDATE_HOLD(Backspace); UPDATE_HOLD(Delete);
        UPDATE_HOLD(Insert); UPDATE_HOLD(Home);
        UPDATE_HOLD(End); UPDATE_HOLD(PageUp);
        UPDATE_HOLD(PageDown);
        UPDATE_HOLD(Shift); UPDATE_HOLD(Control);
        UPDATE_HOLD(Alt);

        UPDATE_HOLD(F1); UPDATE_HOLD(F2);
        UPDATE_HOLD(F3); UPDATE_HOLD(F4);
        UPDATE_HOLD(F5); UPDATE_HOLD(F6);
        UPDATE_HOLD(F7); UPDATE_HOLD(F8);
        UPDATE_HOLD(F9); UPDATE_HOLD(F10);
        UPDATE_HOLD(F11); UPDATE_HOLD(F12);

        KeyIsReleased::Up = Keyboard::isReleased(VK_UP);
        KeyIsReleased::Down = Keyboard::isReleased(VK_DOWN);
        KeyIsReleased::Left = Keyboard::isReleased(VK_LEFT);
        KeyIsReleased::Right = Keyboard::isReleased(VK_RIGHT);
        KeyIsReleased::Escape = Keyboard::isReleased(VK_ESCAPE);
        KeyIsReleased::Enter = Keyboard::isReleased(VK_RETURN);
        KeyIsReleased::Space = Keyboard::isReleased(VK_SPACE);
        KeyIsReleased::Shift = Keyboard::isReleased(VK_SHIFT);
        KeyIsReleased::Control = Keyboard::isReleased(VK_CONTROL);
        KeyIsReleased::Alt = Keyboard::isReleased(VK_MENU);
        KeyIsReleased::L_Mouse = Keyboard::isReleased(VK_LBUTTON);
        KeyIsReleased::R_Mouse = Keyboard::isReleased(VK_RBUTTON);

#undef UPDATE_PRESSED
#undef UPDATE_PRESSED_LETTER
#undef UPDATE_HOLD
#undef UPDATE_HOLD_LETTER
        }


    // ------------------------------------ Additional Features --------------------------------------- //

    // ----- Check if a key is pressed
    inline bool isKeyPressed() {
        for (int i = 0; i < 256; i++) {
            if (Keyboard::isPressed(i)) return true;
            }
        return false;
        }

    // ----- Check if a key is hold
    inline bool isKeyHold() {
        for (int i = 0; i < 256; i++) {
            if (Keyboard::isHold(i)) return true;
            }
        return false;
        }

    // ----- Check if a key is released
    inline bool isKeyReleased() {
        for (int i = 0; i < 256; i++) {
            if (Keyboard::isReleased(i)) return true;
            }
        return false;
        }

    // ----- Wait for any key to be pressed
    inline void waitForAnyKey(const std::string& msg = "Press any key to continue... ") {
        std::cout << msg;
        std::cout.flush();
        _getch();
        std::cout << '\n';
        }

    // ----- Get the last key pressed
    inline int lastKeyPressed() {
        for (int i = 0; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                return i;
                }
            }
        return 0;
        }

    // ----- Check if a letter key is pressed
    inline bool letterIsPressed() {
        for (char c = 'A'; c <= 'Z'; c++) {
            if (GetAsyncKeyState(c) & 0x8000) return true;
            }
        for (char c = 'a'; c <= 'z'; c++) {
            if (GetAsyncKeyState(c) & 0x8000) return true;
            }
        return false;
        }

    // ----- Check if a number key is pressed
    inline bool numberIsPressed() {
        for (char c = '0'; c <= '9'; c++) {
            if (GetAsyncKeyState(c) & 0x8000) return true;
            }
        return false;
        }

    // ----- Check if an arrow key is pressed
    inline bool arrowIsPressed() {
        return (GetAsyncKeyState(VK_UP) & 0x8000) ||
            (GetAsyncKeyState(VK_DOWN) & 0x8000) ||
            (GetAsyncKeyState(VK_LEFT) & 0x8000) ||
            (GetAsyncKeyState(VK_RIGHT) & 0x8000);
        }

    // ----- Check if a modifier key is pressed
    inline bool modifierIsPressed() {
        return (GetAsyncKeyState(VK_CONTROL) & 0x8000) ||
            (GetAsyncKeyState(VK_MENU) & 0x8000) ||
            (GetAsyncKeyState(VK_SHIFT) & 0x8000);
        }

    // ----- Check if a mouse key is pressed
    inline bool isMouseIsPressed() {
        return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ||
            (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
        }

    // ----- Get the name of the pressed key
    inline std::string getKeyName(int key) {
        switch (key) {
            case VK_LEFT:   return "Left Arrow";
            case VK_RIGHT:  return "Right Arrow";
            case VK_UP:     return "Up Arrow";
            case VK_DOWN:   return "Down Arrow";
            case VK_ESCAPE: return "Escape";
            case VK_RETURN: return "Enter";
            case VK_SPACE:  return "Space";
            case VK_TAB:    return "Tab";
            case VK_BACK:   return "Backspace";
            case VK_DELETE: return "Delete";
            case VK_INSERT: return "Insert";
            case VK_HOME:   return "Home";
            case VK_END:    return "End";
            case VK_PRIOR:  return "Page Up";
            case VK_NEXT:   return "Page Down";
            case VK_SHIFT:  return "Shift";
            case VK_CONTROL: return "Control";
            case VK_MENU:   return "Alt";
            case VK_F1:  return "F1";
            case VK_F2:  return "F2";
            case VK_F3:  return "F3";
            case VK_F4:  return "F4";
            case VK_F5:  return "F5";
            case VK_F6:  return "F6";
            case VK_F7:  return "F7";
            case VK_F8:  return "F8";
            case VK_F9:  return "F9";
            case VK_F10: return "F10";
            case VK_F11: return "F11";
            case VK_F12: return "F12";
            case VK_LBUTTON: return "Left Mouse Button";
            case VK_RBUTTON: return "Right Mouse Button";
            default:
                if (key >= 32 && key <= 126) {
                    return std::string(1, static_cast<char>(key));
                    }
                return "Unknown (" + std::to_string(key) + ")";
            }
        }

    // ----- Get the name of the number key pressed
    inline std::string getNumKey() {
        if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000) { return "0"; }
        if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) { return "1"; }
        if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) { return "2"; }
        if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) { return "3"; }
        if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000) { return "4"; }
        if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000) { return "5"; }
        if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000) { return "6"; }
        if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) { return "7"; }
        if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) { return "8"; }
        if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) { return "9"; }
        return "None";
        }

    // ----- Get the name of the modifier key pressed
    inline std::string getModifierKey() {
        bool lctrl = GetAsyncKeyState(VK_LCONTROL) & 0x8000;
        bool rctrl = GetAsyncKeyState(VK_RCONTROL) & 0x8000;
        bool lalt = GetAsyncKeyState(VK_LMENU) & 0x8000;
        bool ralt = GetAsyncKeyState(VK_RMENU) & 0x8000;
        bool lshift = GetAsyncKeyState(VK_LSHIFT) & 0x8000;
        bool rshift = GetAsyncKeyState(VK_RSHIFT) & 0x8000;
        if (lctrl && rctrl) return "Both Control";
        if (lalt && ralt) return "Both Alt";
        if (lshift && rshift) return "Both Shift";
        if (lctrl) return "Left Control";
        if (rctrl) return "Right Control";
        if (lalt) return "Left Alt";
        if (ralt) return "Right Alt";
        if (lshift) return "Left Shift";
        if (rshift) return "Right Shift";
        return "None";
        }

    // ----- Check if caps key is lock on
    inline bool isCapsLockOn() {
        return GetKeyState(VK_CAPITAL) & 0x0001;
        }

    // ----- Check if num key is lock on
    inline bool isNumLockOn() {
        return GetKeyState(VK_NUMLOCK) & 0x0001;
        }

    // ----- Check if a key is down 
    inline bool isKeyDown(char key) {
        return GetAsyncKeyState(key) & 0x8000;
        }

    // ----- Check if Multiple keys are pressed
    template<typename... Keys>
    inline bool areKeysPressed(Keys... keys) {
        bool allPressed = true;
        ((allPressed = allPressed && (GetAsyncKeyState(keys) & 0x8000)), ...);
        return allPressed;
        }
    }
