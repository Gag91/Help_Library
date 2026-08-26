#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>

namespace hp {

    // ----- Saving Data
    template <typename T>
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
    inline bool load(const std::string& filename, const std::vector<std::string>& names, std::vector<std::string>& args) {
        std::ifstream file(filename);
        if (!file.is_open()) [[unlikely]] {
            std::cerr << "Could not open " + filename << "\n";
            return false;
            }

        std::string line;
        for (size_t i = 0; i < args.size() && i < names.size(); i++) {
            if (!std::getline(file, line)) {
                break;
                }

            if (line.rfind(names[i], 0) == 0) {
                args[i] = line.substr(names[i].size() + 2);
                }
            }
        return true;
        }

    // ----- Loading specified data
    template <typename T = std::string>
    inline std::optional<T> load(const std::string& filename, const std::string& arg) {
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
