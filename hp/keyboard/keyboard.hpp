#pragma once

#include "hp/keyboard/hold.hpp"
#include "hp/keyboard/pressed.hpp"
#include "hp/keyboard/released.hpp"
#include "hp/keyboard/repeat.hpp"
#include "hp/platform/console.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#endif

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

    enum class Key {
        A = 'A',
        B = 'B',
        C = 'C',
        D = 'D',
        E = 'E',
        F = 'F',
        G = 'G',
        H = 'H',
        I = 'I',
        J = 'J',
        K = 'K',
        L = 'L',
        M = 'M',
        N = 'N',
        O = 'O',
        P = 'P',
        Q = 'Q',
        R = 'R',
        S = 'S',
        T = 'T',
        U = 'U',
        V = 'V',
        W = 'W',
        X = 'X',
        Y = 'Y',
        Z = 'Z',
        Num0 = '0',
        Num1 = '1',
        Num2 = '2',
        Num3 = '3',
        Num4 = '4',
        Num5 = '5',
        Num6 = '6',
        Num7 = '7',
        Num8 = '8',
        Num9 = '9',
        Escape = 0x1B,
        Enter = 0x0D,
        Space = 0x20,
        Tab = 0x09,
        Backspace = 0x08,
        Delete = 0x2E,
        Insert = 0x2D,
        Home = 0x24,
        End = 0x23,
        PageUp = 0x21,
        PageDown = 0x22,
        Up = 0x26,
        Down = 0x28,
        Left = 0x25,
        Right = 0x27,
        Shift = 0x10,
        Control = 0x11,
        Alt = 0x12,
        F1 = 0x70,
        F2 = 0x71,
        F3 = 0x72,
        F4 = 0x73,
        F5 = 0x74,
        F6 = 0x75,
        F7 = 0x76,
        F8 = 0x77,
        F9 = 0x78,
        F10 = 0x79,
        F11 = 0x7A,
        F12 = 0x7B
    };

    class Keyboard {
      private:
        static inline std::unordered_map<int, bool> previousStates;
        static inline std::unordered_map<int, bool> currentStates;

      public:
        static void update() {
#ifdef _WIN32
            for (int i = 0; i < 256; i++) {
                previousStates[i] = currentStates[i];
                currentStates[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
            }
#endif
        }

        static bool isPressed(int key) {
#ifdef _WIN32
            return currentStates[key] && !previousStates[key];
#else
            (void)key;
            return false;
#endif
        }

        static bool isHold(int key) {
#ifdef _WIN32
            return currentStates[key];
#else
            (void)key;
            return false;
#endif
        }

        static bool isReleased(int key) {
#ifdef _WIN32
            return !currentStates[key] && previousStates[key];
#else
            (void)key;
            return false;
#endif
        }

        static bool isDown(int key) {
            return isHold(key) || isPressed(key);
        }
    };

    inline void clear_pressed_flags() {
        KeyIsPressed::A = KeyIsPressed::B = KeyIsPressed::C = false;
        KeyIsPressed::D = KeyIsPressed::E = KeyIsPressed::F = false;
        KeyIsPressed::G = KeyIsPressed::H = KeyIsPressed::I = false;
        KeyIsPressed::J = KeyIsPressed::K = KeyIsPressed::L = false;
        KeyIsPressed::M = KeyIsPressed::N = KeyIsPressed::O = false;
        KeyIsPressed::P = KeyIsPressed::Q = KeyIsPressed::R = false;
        KeyIsPressed::S = KeyIsPressed::T = KeyIsPressed::U = false;
        KeyIsPressed::V = KeyIsPressed::W = KeyIsPressed::X = false;
        KeyIsPressed::Y = KeyIsPressed::Z = false;
        KeyIsPressed::Num0 = KeyIsPressed::Num1 = false;
        KeyIsPressed::Num2 = KeyIsPressed::Num3 = false;
        KeyIsPressed::Num4 = KeyIsPressed::Num5 = false;
        KeyIsPressed::Num6 = KeyIsPressed::Num7 = false;
        KeyIsPressed::Num8 = KeyIsPressed::Num9 = false;
        KeyIsPressed::Up = KeyIsPressed::Down = false;
        KeyIsPressed::Left = KeyIsPressed::Right = false;
        KeyIsPressed::Escape = KeyIsPressed::Enter = false;
        KeyIsPressed::Space = KeyIsPressed::Tab = false;
        KeyIsPressed::Backspace = KeyIsPressed::Delete = false;
        KeyIsPressed::Insert = KeyIsPressed::Home = false;
        KeyIsPressed::End = KeyIsPressed::PageUp = false;
        KeyIsPressed::PageDown = KeyIsPressed::Shift = false;
        KeyIsPressed::Control = KeyIsPressed::Alt = false;
        KeyIsPressed::F1 = KeyIsPressed::F2 = false;
        KeyIsPressed::F3 = KeyIsPressed::F4 = false;
        KeyIsPressed::F5 = KeyIsPressed::F6 = false;
        KeyIsPressed::F7 = KeyIsPressed::F8 = false;
        KeyIsPressed::F9 = KeyIsPressed::F10 = false;
        KeyIsPressed::F11 = KeyIsPressed::F12 = false;
        KeyIsPressed::L_Mouse = KeyIsPressed::R_Mouse = false;
    }

    inline void set_letter_flag(unsigned char c) {
        switch (c) {
            case 'A':
            case 'a':
                KeyIsPressed::A = true;
                break;
            case 'B':
            case 'b':
                KeyIsPressed::B = true;
                break;
            case 'C':
            case 'c':
                KeyIsPressed::C = true;
                break;
            case 'D':
            case 'd':
                KeyIsPressed::D = true;
                break;
            case 'E':
            case 'e':
                KeyIsPressed::E = true;
                break;
            case 'F':
            case 'f':
                KeyIsPressed::F = true;
                break;
            case 'G':
            case 'g':
                KeyIsPressed::G = true;
                break;
            case 'H':
            case 'h':
                KeyIsPressed::H = true;
                break;
            case 'I':
            case 'i':
                KeyIsPressed::I = true;
                break;
            case 'J':
            case 'j':
                KeyIsPressed::J = true;
                break;
            case 'K':
            case 'k':
                KeyIsPressed::K = true;
                break;
            case 'L':
            case 'l':
                KeyIsPressed::L = true;
                break;
            case 'M':
            case 'm':
                KeyIsPressed::M = true;
                break;
            case 'N':
            case 'n':
                KeyIsPressed::N = true;
                break;
            case 'O':
            case 'o':
                KeyIsPressed::O = true;
                break;
            case 'P':
            case 'p':
                KeyIsPressed::P = true;
                break;
            case 'Q':
            case 'q':
                KeyIsPressed::Q = true;
                break;
            case 'R':
            case 'r':
                KeyIsPressed::R = true;
                break;
            case 'S':
            case 's':
                KeyIsPressed::S = true;
                break;
            case 'T':
            case 't':
                KeyIsPressed::T = true;
                break;
            case 'U':
            case 'u':
                KeyIsPressed::U = true;
                break;
            case 'V':
            case 'v':
                KeyIsPressed::V = true;
                break;
            case 'W':
            case 'w':
                KeyIsPressed::W = true;
                break;
            case 'X':
            case 'x':
                KeyIsPressed::X = true;
                break;
            case 'Y':
            case 'y':
                KeyIsPressed::Y = true;
                break;
            case 'Z':
            case 'z':
                KeyIsPressed::Z = true;
                break;
            case '0':
                KeyIsPressed::Num0 = true;
                break;
            case '1':
                KeyIsPressed::Num1 = true;
                break;
            case '2':
                KeyIsPressed::Num2 = true;
                break;
            case '3':
                KeyIsPressed::Num3 = true;
                break;
            case '4':
                KeyIsPressed::Num4 = true;
                break;
            case '5':
                KeyIsPressed::Num5 = true;
                break;
            case '6':
                KeyIsPressed::Num6 = true;
                break;
            case '7':
                KeyIsPressed::Num7 = true;
                break;
            case '8':
                KeyIsPressed::Num8 = true;
                break;
            case '9':
                KeyIsPressed::Num9 = true;
                break;
            default:
                break;
        }
    }

    inline void updateKeyboard() {
        clear_pressed_flags();
#ifdef _WIN32
        Keyboard::update();

        KeyIsPressed::A = Keyboard::isPressed('A');
        KeyIsPressed::B = Keyboard::isPressed('B');
        KeyIsPressed::C = Keyboard::isPressed('C');
        KeyIsPressed::D = Keyboard::isPressed('D');
        KeyIsPressed::E = Keyboard::isPressed('E');
        KeyIsPressed::F = Keyboard::isPressed('F');
        KeyIsPressed::G = Keyboard::isPressed('G');
        KeyIsPressed::H = Keyboard::isPressed('H');
        KeyIsPressed::I = Keyboard::isPressed('I');
        KeyIsPressed::J = Keyboard::isPressed('J');
        KeyIsPressed::K = Keyboard::isPressed('K');
        KeyIsPressed::L = Keyboard::isPressed('L');
        KeyIsPressed::M = Keyboard::isPressed('M');
        KeyIsPressed::N = Keyboard::isPressed('N');
        KeyIsPressed::O = Keyboard::isPressed('O');
        KeyIsPressed::P = Keyboard::isPressed('P');
        KeyIsPressed::Q = Keyboard::isPressed('Q');
        KeyIsPressed::R = Keyboard::isPressed('R');
        KeyIsPressed::S = Keyboard::isPressed('S');
        KeyIsPressed::T = Keyboard::isPressed('T');
        KeyIsPressed::U = Keyboard::isPressed('U');
        KeyIsPressed::V = Keyboard::isPressed('V');
        KeyIsPressed::W = Keyboard::isPressed('W');
        KeyIsPressed::X = Keyboard::isPressed('X');
        KeyIsPressed::Y = Keyboard::isPressed('Y');
        KeyIsPressed::Z = Keyboard::isPressed('Z');

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

        KeyIsPressed::Up = Keyboard::isPressed(VK_UP);
        KeyIsPressed::Down = Keyboard::isPressed(VK_DOWN);
        KeyIsPressed::Left = Keyboard::isPressed(VK_LEFT);
        KeyIsPressed::Right = Keyboard::isPressed(VK_RIGHT);
        KeyIsPressed::Escape = Keyboard::isPressed(VK_ESCAPE);
        KeyIsPressed::Enter = Keyboard::isPressed(VK_RETURN);
        KeyIsPressed::Space = Keyboard::isPressed(VK_SPACE);
        KeyIsPressed::Tab = Keyboard::isPressed(VK_TAB);
        KeyIsPressed::Backspace = Keyboard::isPressed(VK_BACK);
        KeyIsPressed::Delete = Keyboard::isPressed(VK_DELETE);
        KeyIsPressed::Insert = Keyboard::isPressed(VK_INSERT);
        KeyIsPressed::Home = Keyboard::isPressed(VK_HOME);
        KeyIsPressed::End = Keyboard::isPressed(VK_END);
        KeyIsPressed::PageUp = Keyboard::isPressed(VK_PRIOR);
        KeyIsPressed::PageDown = Keyboard::isPressed(VK_NEXT);
        KeyIsPressed::Shift = Keyboard::isPressed(VK_SHIFT);
        KeyIsPressed::Control = Keyboard::isPressed(VK_CONTROL);
        KeyIsPressed::Alt = Keyboard::isPressed(VK_MENU);
        KeyIsPressed::L_Mouse = Keyboard::isPressed(VK_LBUTTON);
        KeyIsPressed::R_Mouse = Keyboard::isPressed(VK_RBUTTON);

        KeyIsPressed::F1 = Keyboard::isPressed(VK_F1);
        KeyIsPressed::F2 = Keyboard::isPressed(VK_F2);
        KeyIsPressed::F3 = Keyboard::isPressed(VK_F3);
        KeyIsPressed::F4 = Keyboard::isPressed(VK_F4);
        KeyIsPressed::F5 = Keyboard::isPressed(VK_F5);
        KeyIsPressed::F6 = Keyboard::isPressed(VK_F6);
        KeyIsPressed::F7 = Keyboard::isPressed(VK_F7);
        KeyIsPressed::F8 = Keyboard::isPressed(VK_F8);
        KeyIsPressed::F9 = Keyboard::isPressed(VK_F9);
        KeyIsPressed::F10 = Keyboard::isPressed(VK_F10);
        KeyIsPressed::F11 = Keyboard::isPressed(VK_F11);
        KeyIsPressed::F12 = Keyboard::isPressed(VK_F12);
#else
        std::string input = hp::platform::read_all_available();
        std::size_t i = 0;
        while (i < input.size()) {
            unsigned char c = static_cast<unsigned char>(input[i]);
            if (c == 27) {
                if (i + 2 < input.size() && input[i + 1] == '[') {
                    char code = input[i + 2];
                    switch (code) {
                        case 'A':
                            KeyIsPressed::Up = true;
                            break;
                        case 'B':
                            KeyIsPressed::Down = true;
                            break;
                        case 'C':
                            KeyIsPressed::Right = true;
                            break;
                        case 'D':
                            KeyIsPressed::Left = true;
                            break;
                        case 'H':
                            KeyIsPressed::Home = true;
                            break;
                        case 'F':
                            KeyIsPressed::End = true;
                            break;
                        default:
                            break;
                    }
                    i += 3;
                } else {
                    KeyIsPressed::Escape = true;
                    i += 1;
                }
            } else if (c == '\r' || c == '\n') {
                KeyIsPressed::Enter = true;
                i += 1;
            } else if (c == '\t') {
                KeyIsPressed::Tab = true;
                i += 1;
            } else if (c == 127 || c == 8) {
                KeyIsPressed::Backspace = true;
                i += 1;
            } else if (c == ' ') {
                KeyIsPressed::Space = true;
                i += 1;
            } else {
                set_letter_flag(c);
                i += 1;
            }
        }
#endif
    }

    inline bool isKeyPressed() {
        return KeyIsPressed::A || KeyIsPressed::B || KeyIsPressed::C ||
               KeyIsPressed::D || KeyIsPressed::E || KeyIsPressed::F ||
               KeyIsPressed::G || KeyIsPressed::H || KeyIsPressed::I ||
               KeyIsPressed::J || KeyIsPressed::K || KeyIsPressed::L ||
               KeyIsPressed::M || KeyIsPressed::N || KeyIsPressed::O ||
               KeyIsPressed::P || KeyIsPressed::Q || KeyIsPressed::R ||
               KeyIsPressed::S || KeyIsPressed::T || KeyIsPressed::U ||
               KeyIsPressed::V || KeyIsPressed::W || KeyIsPressed::X ||
               KeyIsPressed::Y || KeyIsPressed::Z ||
               KeyIsPressed::Num0 || KeyIsPressed::Num1 || KeyIsPressed::Num2 ||
               KeyIsPressed::Num3 || KeyIsPressed::Num4 || KeyIsPressed::Num5 ||
               KeyIsPressed::Num6 || KeyIsPressed::Num7 || KeyIsPressed::Num8 ||
               KeyIsPressed::Num9 ||
               KeyIsPressed::Up || KeyIsPressed::Down || KeyIsPressed::Left ||
               KeyIsPressed::Right || KeyIsPressed::Escape || KeyIsPressed::Enter ||
               KeyIsPressed::Space || KeyIsPressed::Tab || KeyIsPressed::Backspace ||
               KeyIsPressed::Delete || KeyIsPressed::Insert || KeyIsPressed::Home ||
               KeyIsPressed::End || KeyIsPressed::PageUp || KeyIsPressed::PageDown ||
               KeyIsPressed::Shift || KeyIsPressed::Control || KeyIsPressed::Alt;
    }

    inline bool isKeyHold() {
        return KeyIsHold::A || KeyIsHold::B || KeyIsHold::C ||
               KeyIsHold::D || KeyIsHold::E || KeyIsHold::F ||
               KeyIsHold::G || KeyIsHold::H || KeyIsHold::I ||
               KeyIsHold::J || KeyIsHold::K || KeyIsHold::L ||
               KeyIsHold::M || KeyIsHold::N || KeyIsHold::O ||
               KeyIsHold::P || KeyIsHold::Q || KeyIsHold::R ||
               KeyIsHold::S || KeyIsHold::T || KeyIsHold::U ||
               KeyIsHold::V || KeyIsHold::W || KeyIsHold::X ||
               KeyIsHold::Y || KeyIsHold::Z ||
               KeyIsHold::Up || KeyIsHold::Down || KeyIsHold::Left ||
               KeyIsHold::Right || KeyIsHold::Escape || KeyIsHold::Enter;
    }

    inline bool isKeyReleased() {
        return KeyIsReleased::Up || KeyIsReleased::Down ||
               KeyIsReleased::Left || KeyIsReleased::Right ||
               KeyIsReleased::Escape || KeyIsReleased::Enter;
    }

    inline void waitForAnyKey(const std::string &msg = "Press any key to continue... ") {
        std::cout << msg;
        std::cout.flush();
        hp::platform::getch();
        std::cout << '\n';
        hp::platform::disable_raw();
    }

    inline int lastKeyPressed() {
#ifdef _WIN32
        for (int i = 0; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                return i;
            }
        }
#endif
        return 0;
    }

    inline bool letterIsPressed() {
#ifdef _WIN32
        for (char c = 'A'; c <= 'Z'; c++) {
            if (GetAsyncKeyState(c) & 0x8000)
                return true;
        }
        for (char c = 'a'; c <= 'z'; c++) {
            if (GetAsyncKeyState(c) & 0x8000)
                return true;
        }
#endif
        return false;
    }

    inline bool numberIsPressed() {
#ifdef _WIN32
        for (char c = '0'; c <= '9'; c++) {
            if (GetAsyncKeyState(c) & 0x8000)
                return true;
        }
#endif
        return false;
    }

    inline bool arrowIsPressed() {
#ifdef _WIN32
        return (GetAsyncKeyState(VK_UP) & 0x8000) ||
               (GetAsyncKeyState(VK_DOWN) & 0x8000) ||
               (GetAsyncKeyState(VK_LEFT) & 0x8000) ||
               (GetAsyncKeyState(VK_RIGHT) & 0x8000);
#else
        return KeyIsPressed::Up || KeyIsPressed::Down ||
               KeyIsPressed::Left || KeyIsPressed::Right;
#endif
    }

    inline bool modifierIsPressed() {
#ifdef _WIN32
        return (GetAsyncKeyState(VK_CONTROL) & 0x8000) ||
               (GetAsyncKeyState(VK_MENU) & 0x8000) ||
               (GetAsyncKeyState(VK_SHIFT) & 0x8000);
#else
        return false;
#endif
    }

    inline bool isMouseIsPressed() {
#ifdef _WIN32
        return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ||
               (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
#else
        return false;
#endif
    }

    inline std::string getKeyName(int key) {
        switch (key) {
            case 0x25:
                return "Left Arrow";
            case 0x27:
                return "Right Arrow";
            case 0x26:
                return "Up Arrow";
            case 0x28:
                return "Down Arrow";
            case 0x1B:
                return "Escape";
            case 0x0D:
                return "Enter";
            case 0x20:
                return "Space";
            case 0x09:
                return "Tab";
            case 0x08:
                return "Backspace";
            case 0x2E:
                return "Delete";
            case 0x2D:
                return "Insert";
            case 0x24:
                return "Home";
            case 0x23:
                return "End";
            case 0x21:
                return "Page Up";
            case 0x22:
                return "Page Down";
            case 0x10:
                return "Shift";
            case 0x11:
                return "Control";
            case 0x12:
                return "Alt";
            case 0x70:
                return "F1";
            case 0x71:
                return "F2";
            case 0x72:
                return "F3";
            case 0x73:
                return "F4";
            case 0x74:
                return "F5";
            case 0x75:
                return "F6";
            case 0x76:
                return "F7";
            case 0x77:
                return "F8";
            case 0x78:
                return "F9";
            case 0x79:
                return "F10";
            case 0x7A:
                return "F11";
            case 0x7B:
                return "F12";
            default:
                if (key >= 32 && key <= 126) {
                    return std::string(1, static_cast<char>(key));
                }
                return "Unknown (" + std::to_string(key) + ")";
        }
    }

    inline std::string getNumKey() {
#ifdef _WIN32
        if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
            return "0";
        if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
            return "1";
        if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
            return "2";
        if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
            return "3";
        if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
            return "4";
        if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
            return "5";
        if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
            return "6";
        if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
            return "7";
        if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
            return "8";
        if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000)
            return "9";
#endif
        return "None";
    }

    inline std::string getModifierKey() {
#ifdef _WIN32
        bool lctrl = GetAsyncKeyState(VK_LCONTROL) & 0x8000;
        bool rctrl = GetAsyncKeyState(VK_RCONTROL) & 0x8000;
        bool lalt = GetAsyncKeyState(VK_LMENU) & 0x8000;
        bool ralt = GetAsyncKeyState(VK_RMENU) & 0x8000;
        bool lshift = GetAsyncKeyState(VK_LSHIFT) & 0x8000;
        bool rshift = GetAsyncKeyState(VK_RSHIFT) & 0x8000;
        if (lctrl && rctrl)
            return "Both Control";
        if (lalt && ralt)
            return "Both Alt";
        if (lshift && rshift)
            return "Both Shift";
        if (lctrl)
            return "Left Control";
        if (rctrl)
            return "Right Control";
        if (lalt)
            return "Left Alt";
        if (ralt)
            return "Right Alt";
        if (lshift)
            return "Left Shift";
        if (rshift)
            return "Right Shift";
#endif
        return "None";
    }

    inline bool isCapsLockOn() {
#ifdef _WIN32
        return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
#else
        return false;
#endif
    }

    inline bool isNumLockOn() {
#ifdef _WIN32
        return (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;
#else
        return false;
#endif
    }

    inline bool isKeyDown(char key) {
#ifdef _WIN32
        return (GetAsyncKeyState(key) & 0x8000) != 0;
#else
        (void)key;
        return false;
#endif
    }

    template <typename... Keys>
    inline bool areKeysPressed(Keys... keys) {
#ifdef _WIN32
        bool allPressed = true;
        ((allPressed = allPressed && (GetAsyncKeyState(keys) & 0x8000)), ...);
        return allPressed;
#else
        (void)std::initializer_list<int>{((void)keys, 0)...};
        return false;
#endif
    }
} // namespace hp