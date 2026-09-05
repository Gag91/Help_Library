#pragma once

#include <string>
#include <cstdlib>
#include <stdexcept>
#include <memory>
#include <array>
#include <cstring>

namespace hp {

    inline std::string command(const std::string& command,bool std_err = true) {
        std::array<char, 4096> buffer;

        std::string cmd;
        if(std_err) {
             cmd = command + " 2>&1";
        }

        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("hp::command: Failed to open pipe for command: " + command);
        }

        std::string result;
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            if (result.size() + strlen(buffer.data()) > 4096) {
                throw std::runtime_error("hp::command: Output exceeds buffer size for command: " + command);
            }
            result += buffer.data();
        }
        return result;
    }
}