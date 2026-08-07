#pragma once
#include "hp/keyboard/keyboard.hpp"

namespace hp {
    // ----- Main class
    class KeyRepeat {
    private:
        std::unordered_map<int, int> repeatCounts;
        std::unordered_map<int, bool> wasKeyDown;

    public:

        // ----- Check if key has been repeated
        bool isKeyRepeated(int key, int delay = 500, int repeatDelay = 100) {
            bool isDown = GetAsyncKeyState(key) & 0x8000;
            if (isDown && !wasKeyDown[key]) {
                wasKeyDown[key] = true;
                repeatCounts[key] = 0;
                return false;
                }
            if (isDown) {
                repeatCounts[key]++;
                if (repeatCounts[key] > delay / 10) {
                    if ((repeatCounts[key] - delay / 10) % (repeatDelay / 10) == 0) {
                        return true;
                        }
                    }
                }
            else {
                wasKeyDown[key] = false;
                repeatCounts[key] = 0;
                }
            return false;
            }

        // ----- Reset Count
        void reset() {
            repeatCounts.clear();
            wasKeyDown.clear();
            }
        };
    }