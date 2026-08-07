#pragma once

#include <iostream>
#include <windows.h>
#include <chrono>
#include <ctime>
#include <random>
#include <thread>

namespace hp {

    // ----- Wait in seconds
    inline void wait(double t) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(t * 1000)));
        }

    // ----- Wait in miliseconds
    inline void wait_ms(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }

    // ----- Start a Timer
    inline std::chrono::steady_clock::time_point startTimer() {
        return std::chrono::steady_clock::now();
        }
    // ----- Stop a Timer and get the time elapsed
    inline double stopTimer(std::chrono::steady_clock::time_point start) {
        auto end = std::chrono::steady_clock::now();
        return std::chrono::duration<double>(end - start).count();
        }
    // ----- Display a timer
    inline void displayTimer(std::chrono::steady_clock::time_point start, const std::string& label) {
        double elapsed = stopTimer(start);
        std::cout << label << ": " << elapsed << "s\n";
        }
    }