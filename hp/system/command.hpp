#pragma once

#include "hp/platform/console.hpp"

#include <string>

namespace hp {

    inline std::string command(const std::string &command, bool std_err = true) {
        std::string cmd = command;
        if (std_err) {
            cmd += " 2>&1";
        }
        return hp::platform::popen_exec(cmd);
    }
} // namespace hp