#pragma once

#include "hp/colors/color.hpp"

#include <chrono>
#include <ctime>
#include <format>
#include <print>
#include <string>
#include <utility>

namespace hp {

    class Logger {
      private:
        static std::string timestamp() {
            return std::format("[{:%H:%M:%S}]", std::chrono::system_clock::now());
        }

        template <typename... Args>
        static void emit(const char *color, const char *label, Args &&...args) {
            std::print("{} {}{}{} ", timestamp(), color, label, getColorCode(RESET));
            (std::print("{}", std::forward<Args>(args)), ...);
            std::println();
        }

      public:
        Logger() = default;

        template <typename... Args>
        void warning(Args &&...args) const {
            emit(getColorCode(YELLOW), "[Warning]", std::forward<Args>(args)...);
        }

        template <typename... Args>
        void error(Args &&...args) const {
            emit(getColorCode(RED), "[Error]", std::forward<Args>(args)...);
        }

        template <typename... Args>
        void info(Args &&...args) const {
            emit(getColorCode(BLUE), "[Info]", std::forward<Args>(args)...);
        }

        template <typename... Args>
        void debug(Args &&...args) const {
            emit(getColorCode(MAGENTA), "[Debug]", std::forward<Args>(args)...);
        }

        template <typename... Args>
        void success(Args &&...args) const {
            emit(getColorCode(GREEN), "[Success]", std::forward<Args>(args)...);
        }

        template <typename... Args>
        void trace(Args &&...args) const {
            emit(getColorCode(CYAN), "[Trace]", std::forward<Args>(args)...);
        }

        template <typename... Args>
        void critical(Args &&...args) const {
            emit(getColorCode(RED), "[Critical]", std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log(Args &&...args) const {
            emit("", "[Log]", std::forward<Args>(args)...);
        }
    };

} // namespace hp