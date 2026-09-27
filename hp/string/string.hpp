#pragma once
#include "hp/string/inputs.hpp"
#include <filesystem>
#include <functional>
#include <iostream>
#include <windows.h>

namespace hp {

    namespace str {

        // ----- String to upper
        [[nodiscard]] inline std::string to_upper(std::string_view str) {
            std::string result;
            result.reserve(str.size());
            for (char c : str) {
                result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
            }
            return result;
        }

        // ----- String to lower
        [[nodiscard]] inline std::string to_lower(std::string_view str) {
            std::string result;
            result.reserve(str.size());
            for (char c : str) {
                result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
            }
            return result;
        }

        // ----- String to T
        template <typename T>
        T from_string(const std::string &str) {
            using DecayedT = std::remove_cvref_t<T>;

            if constexpr (std::is_same_v<DecayedT, std::string>) {
                return str;

            } else if constexpr (std::is_same_v<DecayedT, std::string_view>) {
                return std::string_view(str);

            } else if constexpr (std::is_same_v<DecayedT, std::filesystem::path>) {
                return std::filesystem::path(str);

            } else if constexpr (std::is_same_v<DecayedT, char>) {
                if (str.empty()) {
                    throw std::invalid_argument("Empty string cannot be converted to char");
                }
                return str[0];

            } else if constexpr (std::is_same_v<DecayedT, bool>) {
                if (str == "true" || str == "1" || str == "TRUE" || str == "True")
                    return true;
                if (str == "false" || str == "0" || str == "FALSE" || str == "False")
                    return false;
                throw std::invalid_argument("Invalid boolean string: " + str);

            } else if constexpr (std::is_same_v<DecayedT, int>) {
                return std::stoi(str);

            } else if constexpr (std::is_same_v<DecayedT, long>) {
                return std::stol(str);

            } else if constexpr (std::is_same_v<DecayedT, long long>) {
                return std::stoll(str);

            } else if constexpr (std::is_same_v<DecayedT, unsigned long>) {
                return std::stoul(str);

            } else if constexpr (std::is_same_v<DecayedT, unsigned long long>) {
                return std::stoull(str);

            } else if constexpr (std::is_same_v<DecayedT, float>) {
                return std::stof(str);

            } else if constexpr (std::is_same_v<DecayedT, double>) {
                return std::stod(str);

            } else if constexpr (std::is_same_v<DecayedT, long double>) {
                return std::stold(str);

            } else if constexpr (std::is_same_v<DecayedT, short>) {
                int val = std::stoi(str);
                return static_cast<short>(val);

            } else if constexpr (std::is_same_v<DecayedT, unsigned short>) {
                unsigned long val = std::stoul(str);
                return static_cast<unsigned short>(val);

            } else if constexpr (std::is_same_v<DecayedT, int8_t> || std::is_same_v<DecayedT, signed char>) {
                int val = std::stoi(str);
                return static_cast<int8_t>(val);

            } else if constexpr (std::is_same_v<DecayedT, uint8_t> || std::is_same_v<DecayedT, unsigned char>) {
                unsigned long val = std::stoul(str);
                return static_cast<uint8_t>(val);

            } else {
                throw std::runtime_error("Unsupported type");
            }
        }

        namespace trim {

            // ----- White spaces
            [[nodiscard]] inline std::string white_spaces(std::string_view str) {
                std::string result;
                result.reserve(str.size());
                for (char c : str) {
                    if (c != ' ') {
                        result += c;
                    }
                }
                return result;
            }

            // ----- Numbers
            [[nodiscard]] inline std::string numbers(std::string_view str) {
                std::string result;
                result.reserve(str.size());
                for (char c : str) {
                    if (std::isdigit(static_cast<unsigned char>(c))) {
                        result += c;
                    }
                }
                return result;
            }

            // ----- Letters
            [[nodiscard]] inline std::string letters(std::string_view str) {
                std::string result;
                result.reserve(str.size());
                for (char c : str) {
                    if (std::isalpha(static_cast<unsigned char>(c))) {
                        result += c;
                    }
                }
                return result;
            }
        } // namespace trim
    } // namespace str

    // ----- Regular title function
    inline void title(const std::string &t, int w = 40) {
        std::string sep = std::string(w, '=');
        std::cout << sep << std::endl;
        std::cout << t << std::endl;
        std::cout << sep << std::endl;
    }
    // ----- Works the same as "std::string(int, char)" but maded to use string instead
    inline std::string repeatString(const std::string &str, int count) {
        std::string result;
        for (int i = 0; i < count; i++) {
            result += str;
        }
        return result;
    }

    // ----- Centered Seperator
    inline void sp(const std::string &msg, char a = '=') {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int msgLen = msg.length() + 2;
        int totalWidth = consoleWidth;
        int sideLen = (totalWidth - msgLen) / 2;

        std::string result;
        result.reserve(totalWidth);
        result.append(sideLen, a);
        result += " " + msg + " ";
        result.append(sideLen, a);
        if (result.length() < totalWidth) {
            result += a;
        }
        std::cout << "\033[2K\r";
        std::cout << result << '\n';
    }
    // ----- Wait for enter key to be pressed
    inline void waitForEnter(std::function<void()> func = nullptr) {
        std::cout << "Press Enter To Continue..." << std::endl;
        std::cout.flush();
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        if (func) {
            func();
        }
    }

    inline bool starts_with(const std::string &str, const std::string &prefix) {
        return str.starts_with(prefix);
    }

    inline bool ends_with(const std::string &str, const std::string &suffix) {
        return str.ends_with(suffix);
    }

} // namespace hp