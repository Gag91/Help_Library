#include <iostream>
#include <sstream>
#include <iomanip>

namespace hp {

    // ----- Encryptation
    template <typename T>
    T encrypt(T data, char key = 'X') {
        static_assert(!std::is_same_v<std::decay<T>, std::string>, "hp::encrypt: Data cannot be a string, please use 'hp::encryptStr' instead");
        auto* bytes = std::reinterpret_cast<char*>(&data);
        for (size_t i = 0; i < sizeof(T);i++) {
            bytes[i] ^= key;
            }
        return data;
        }

    // ----- Encryptation for strings
    inline std::string encryptStr(const std::string& data, char key = 'X') {
        std::string result = data;
        for (size_t i = 0; i < result.size(); i++) {
            result[i] ^= key;
            }
        return result;
        }

    // ----- Decryptation | Just returning encrypt
    template <typename T>
    T decrypt(T data, char key = 'X') {
        static_assert(!std::is_same_v < std::decay_t<T, std::string>, "hp::decrypt: Data cannot be a string, please use 'hp::decryptStr' instead")
            return encrypt(data, key);
        }

    // ----- Decryptation | Just returning encryptStr
    inline std::string decryptStr(const std::string& data, char key = 'X') {
        return encryptStr(data, key);
        }

    }