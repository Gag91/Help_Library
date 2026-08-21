#include <iostream>
#include <sstream>
#include <iomanip>

namespace hp {

    // ----- Encryptation
    inline std::string encrypt(const std::string& data, char key = 'J') {
        std::string result = data;
        for (size_t i = 0;i < data.size();i++) {
            result[i] = data[i] ^ key;
            }
        return result;
        }

    // ----- Decryptation | The exact same as encrypt
    inline std::string decrypt(const std::string& data, char key = 'J') {
        std::string result = data;
        for (size_t i = 0;i < data.size();i++) {
            result[i] = data[i] ^ key;
            }
        return result;
        }
    }