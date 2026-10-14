#pragma once

#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace hp {

    // ----- Saving Data
    template <typename T = std::string>
        requires std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
    bool save(const std::string &filename, const std::vector<std::pair<std::string, T>> &args) {
        std::ofstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            return false;
        }

        for (size_t i = 0; i < args.size(); i++) {
            file << args[i].first << ": " << args[i].second << "\n";
        }

        return file.good();
    }

    // ----- Loading Data
    template <typename T = std::string>
        requires std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
    bool load(const std::string &filename, const std::vector<std::pair<std::string, T *>> &arg) {
        std::ifstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            for (size_t i = 0; i < arg.size(); i++) {
                if (line.rfind(arg[i].first + ": ", 0) == 0) {
                    std::string value = line.substr(arg[i].first.size() + 2);
                    if constexpr (std::is_same_v<T, std::string>) {
                        *arg[i].second = value;
                    } else {
                        std::stringstream ss(value);
                        ss >> *arg[i].second;
                    }
                    break;
                }
            }
        }
        return true;
    }

    // ----- Loading specified data
    template <typename T = std::string>
        requires std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
    std::optional<T> load(const std::string &filename, const std::string &arg) {
        std::ifstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            return std::nullopt;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.rfind(arg + ": ", 0) == 0) {
                std::string value = line.substr(arg.size() + 2);

                if constexpr (std::is_same_v<T, std::string>) {
                    return value;
                } else {
                    std::stringstream ss(value);
                    T result;
                    if (ss >> result) {
                        return result;
                    }
                    return std::nullopt;
                }
            }
        }
        return std::nullopt;
    }

} // namespace hp