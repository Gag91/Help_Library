#pragma once

#include "hp/platform/console.hpp"

#include <unordered_map>

namespace hp {
    class KeyRepeat {
      private:
        static inline std::unordered_map<int, int> repeatCounts;
        static inline std::unordered_map<int, bool> wasKeyDown;

      public:
        inline bool isKeyRepeated(int key, int delay = 500, int repeatDelay = 100) {
#ifdef _WIN32
            bool isDown = (GetAsyncKeyState(key) & 0x8000) != 0;
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
            } else {
                wasKeyDown[key] = false;
                repeatCounts[key] = 0;
            }
            return false;
#else
            (void)key;
            (void)delay;
            (void)repeatDelay;
            return false;
#endif
        }

        inline void reset() {
            repeatCounts.clear();
            wasKeyDown.clear();
        }
    };
} // namespace hp