#pragma once

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>

#else
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace hp {
    namespace platform {

        inline void enable_ansi() {
#ifdef _WIN32
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD mode = 0;
            if (GetConsoleMode(hOut, &mode)) {
                SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
#endif
        }

        namespace detail {
            inline bool &raw_active() {
                static bool active = false;
                return active;
            }
#ifndef _WIN32
            inline struct termios &saved_termios() {
                static struct termios t{};
                return t;
            }
#endif
        } // namespace detail

        inline void ensure_raw() {
#ifndef _WIN32
            if (detail::raw_active()) {
                return;
            }
            tcgetattr(STDIN_FILENO, &detail::saved_termios());
            struct termios raw = detail::saved_termios();
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_cc[VMIN] = 0;
            raw.c_cc[VTIME] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &raw);
            detail::raw_active() = true;
            static bool registered = false;
            if (!registered) {
                registered = true;
                std::atexit([]() {
                    if (detail::raw_active()) {
                        tcsetattr(STDIN_FILENO, TCSANOW, &detail::saved_termios());
                        detail::raw_active() = false;
                    }
                });
            }
#endif
        }

        inline void disable_raw() {
#ifndef _WIN32
            if (!detail::raw_active()) {
                return;
            }
            tcsetattr(STDIN_FILENO, TCSANOW, &detail::saved_termios());
            detail::raw_active() = false;
#endif
        }

        inline int console_width() {
#ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
                return csbi.srWindow.Right - csbi.srWindow.Left + 1;
            }
            return 80;
#else
            struct winsize w;
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
                return w.ws_col;
            }
            return 80;
#endif
        }

        inline int console_height() {
#ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
                return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            }
            return 25;
#else
            struct winsize w;
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_row > 0) {
                return w.ws_row;
            }
            return 25;
#endif
        }

        inline void sleep_ms(int ms) {
#ifdef _WIN32
            Sleep(static_cast<DWORD>(ms));
#else
            usleep(static_cast<useconds_t>(ms) * 1000);
#endif
        }

        inline void clear_screen() {
            std::fputs("\033[2J\033[H", stdout);
            std::fflush(stdout);
        }

        inline bool kbhit() {
#ifdef _WIN32
            return _kbhit() != 0;
#else
            ensure_raw();
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            struct timeval tv{0, 0};
            return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
#endif
        }

        inline int getch() {
#ifdef _WIN32
            return _getch();
#else
            ensure_raw();
            unsigned char c;
            if (read(STDIN_FILENO, &c, 1) != 1) {
                return -1;
            }
            return static_cast<int>(c);
#endif
        }

        inline void flush_input() {
#ifdef _WIN32
            while (_kbhit()) {
                _getch();
            }
#else
            ensure_raw();
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            struct timeval tv{0, 0};
            while (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0) {
                char buf[64];
                if (read(STDIN_FILENO, buf, sizeof(buf)) <= 0) {
                    break;
                }
                FD_ZERO(&fds);
                FD_SET(STDIN_FILENO, &fds);
                tv = {0, 0};
            }
#endif
        }

        inline std::string read_all_available() {
            std::string result;
#ifndef _WIN32
            ensure_raw();
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            struct timeval tv{0, 0};
            while (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0) {
                char buf[64];
                ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
                if (n <= 0) {
                    break;
                }
                result.append(buf, static_cast<std::size_t>(n));
                FD_ZERO(&fds);
                FD_SET(STDIN_FILENO, &fds);
                tv = {0, 0};
            }
#endif
            return result;
        }

        inline std::string popen_exec(const std::string &cmd) {
#ifdef _WIN32
            FILE *pipe = _popen(cmd.c_str(), "r");
#else
            FILE *pipe = popen(cmd.c_str(), "r");
#endif
            if (!pipe) {
                throw std::runtime_error("hp::platform::popen_exec: failed to open pipe for: " + cmd);
            }
            std::string result;
            char buffer[4096];
            while (std::fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                result += buffer;
            }
#ifdef _WIN32
            _pclose(pipe);
#else
            pclose(pipe);
#endif
            return result;
        }

        inline void enable_utf8() {
#ifdef _WIN32
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
#endif
            enable_ansi();
        }

        struct ansi_initializer {
            ansi_initializer() {
                enable_ansi();
            }
        };
        [[maybe_unused]] inline ansi_initializer ansi_init_instance;
    } // namespace platform
} // namespace hp