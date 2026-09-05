#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>

namespace hp {

    // ----- Saving Data
    template <typename T = std::string>
    requires std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
    bool save(const std::string& filename, const std::vector<std::pair<std::string, T>>& args) {
        std::ofstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            std::cerr << "Could not open " << filename << "\n";
            return false;
            }

        for (size_t i = 0; i < args.size(); i++) {
            file << args[i].first << ": " << args[i].second << "\n";
            }

        return file.good();
        }


    // ----- Loading Data
    template <typename T = std::string>
    requires std::is_same_v<T, std::string>
    bool load(const std::string& filename, const std::vector<std::pair<std::string, T*>>& arg) {
        auto& args = arg;
        std::ifstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            std::cerr << "Could not open " + filename << "\n";
            return false;
            }

        std::string line;
        for (size_t i = 0; i < args.size(); i++) {
            if (!std::getline(file, line)) {
                break;
                }
            if (line.rfind(args[i].first, 0) == 0) { 
                *args[i].second = line.substr(args[i].first.size() + 2);
            }
            }
        return true;
        }

    // ----- Loading specified data
    template <typename T = std::string>
    requires std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
    std::optional<T> load(const std::string& filename, const std::string& arg) {
        std::ifstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            std::cerr << "Could not open " + filename << "\n";
            return std::nullopt;
            }

        std::string line;
        std::string value;
        while (std::getline(file, line)) {
            if (line.find(arg) != std::string::npos) {
                value = line.substr(arg.size() + 2);

                std::stringstream ss(value);
                T result;
                if (ss >> result) {
                    return result;
                    }
                return std::nullopt;
                }
            }
        return std::nullopt;
        }

    } // namespace hp
