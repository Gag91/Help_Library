#pragma once

#include "hp/colors/color.hpp"
#include "hp/other/print.hpp"
#include "hp/platform/console.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

namespace hp {

    inline std::string getIp() {
        try {
            std::string result = hp::platform::popen_exec("curl -s ifconfig.me");
            std::erase(result, '\n');
            return result.empty() ? "ERROR" : result;
        } catch (...) {
            return "ERROR";
        }
    }

    inline void delln(bool currentln = true) {
        if (currentln) {
            std::cout << "\33[2K" << std::flush;
        } else {
            std::cout << "\33[1A\33[2K" << std::flush;
        }
    }

    inline void cls() {
        hp::platform::clear_screen();
    }

    inline void clsClear() {
        std::cout << "\033[2J\033[H" << std::flush;
    }

    class File {
      public:
        bool create(const std::string &file) {
            std::ofstream out(file);
            return out.is_open();
        }

        bool del(const std::string &file) {
            return fs::remove(file);
        }

        bool write(const std::string &file, const std::string &content) {
            std::ofstream out(file);
            if (!out.is_open())
                return false;
            out << content;
            out.close();
            return true;
        }

        bool f_append(const std::string &file, const std::string &content) {
            std::ofstream out(file, std::ios::app);
            if (!out.is_open())
                return false;
            out << content;
            out.close();
            return true;
        }

        std::string read(const std::string &file) const {
            std::ifstream in(file);
            if (!in.is_open())
                return "";
            std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            in.close();
            return content;
        }

        bool exists(const std::string &file) const {
            return fs::exists(file);
        }

        std::size_t getSize(const std::string &file) const {
            return fs::file_size(file);
        }
    };

    class Folder {
      public:
        void create(const std::string &path) {
            fs::create_directories(path);
        }

        void del(const std::string &path) {
            fs::remove(path);
        }

        void delAll(const std::string &path) {
            fs::remove_all(path);
        }

        bool exists(const std::string &path) {
            return fs::is_directory(path);
        }
    };

    inline void printCl(const std::string &msg, Color color = WHITE) {
        std::cout << getColorCode(color) << msg << "\033[0m";
    }

    inline void printlnCl(const std::string &msg, Color color = WHITE) {
        std::cout << getColorCode(color) << msg << "\033[0m\n";
    }

    inline void exit(int a = 0) {
        if (a == 0) {
            std::exit(0);
        } else if (a == 1) {
            std::exit(1);
        } else {
            throw std::runtime_error("Integer must be 0 or 1");
        }
    }

    template <typename T>
    inline void loop(int count, T action) {
        for (int i = 0; i < count; i++) {
            action(i);
        }
    }

    inline void setCenter(int row, int width) {
        int consoleWidth = hp::platform::console_width();
        int col = (consoleWidth - width) / 2;
        std::cout << "\033[" << row << ";" << col << "H";
    }

    inline void enableUTF8() {
        hp::platform::enable_utf8();
    }
} // namespace hp