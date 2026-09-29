#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace hp::ser {

    enum class TokenType {
        LeftBrace,    // {
        RightBrace,   // }
        LeftBracket,  // [
        RightBracket, // ]
        Colon,        // :
        Comma,        // ,
        String,       // "hello"
        Number,       // 42, -3.14, 1e10
        True,         // true
        False,        // false
        Null,         // null
        Error,        // Error
        EndOfFile,    // end of input
    };

    struct Token {
        TokenType type;
        std::string text;
        std::size_t line;
        std::size_t column;
    };

    std::string_view token_type_name(TokenType type);

    inline std::string_view token_type_name(TokenType type) {
        switch (type) {
            case TokenType::LeftBrace:
                return "LeftBrace";
            case TokenType::RightBrace:
                return "RightBrace";
            case TokenType::LeftBracket:
                return "LeftBracket";
            case TokenType::RightBracket:
                return "RightBracket";
            case TokenType::Colon:
                return "Colon";
            case TokenType::Comma:
                return "Comma";
            case TokenType::String:
                return "String";
            case TokenType::Number:
                return "Number";
            case TokenType::True:
                return "True";
            case TokenType::False:
                return "False";
            case TokenType::Null:
                return "Null";
            case TokenType::Error:
                return "Error";
            case TokenType::EndOfFile:
                return "EndOfFile";
        }
        return "Unknown";
    }

} // namespace hp::ser