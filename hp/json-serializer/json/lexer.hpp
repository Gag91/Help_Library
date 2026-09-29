#pragma once

#include "tokens.hpp"

#include <cctype>
#include <format>
#include <string_view>

namespace hp::ser {

    class Lexer {
      public:
        explicit Lexer(std::string_view source) : source_(source) {}

        Token next();

      private:
        std::string_view source_;
        std::size_t position_ = 0;
        std::size_t line_ = 1;
        std::size_t column_ = 1;

        inline char current() const;
        inline char peek() const;
        inline void advance();
        inline void skip_whitespace();
    };

    inline char Lexer::current() const {
        if (position_ >= source_.size())
            return '\0';
        return source_[position_];
    }

    inline char Lexer::peek() const {
        if (position_ + 1 >= source_.size())
            return '\0';
        return source_[position_ + 1];
    }

    inline void Lexer::advance() {
        if (position_ >= source_.size())
            return;
        if (source_[position_] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        position_++;
    }
    inline void Lexer::skip_whitespace() {
        while (position_ < source_.size() && std::isspace(static_cast<unsigned char>(current()))) {
            advance();
        }
    }

    inline Token Lexer::next() {
        skip_whitespace();

        std::size_t start_line = line_;
        std::size_t start_column = column_;
        char c = current();

        if (c == '\0') {
            return Token{TokenType::EndOfFile, "", start_line, start_column};
        }

        switch (c) {
            case '{':
                advance();
                return Token{TokenType::LeftBrace, "{", start_line, start_column};
            case '}':
                advance();
                return Token{TokenType::RightBrace, "}", start_line, start_column};
            case '[':
                advance();
                return Token{TokenType::LeftBracket, "[", start_line, start_column};
            case ']':
                advance();
                return Token{TokenType::RightBracket, "]", start_line, start_column};
            case ':':
                advance();
                return Token{TokenType::Colon, ":", start_line, start_column};
            case ',':
                advance();
                return Token{TokenType::Comma, ",", start_line, start_column};

            case '"': {
                advance();

                std::string value;
                while (current() != '"' && current() != '\0') {
                    if (current() == '\\') {
                        advance();
                        switch (current()) {
                            case '"':
                                value += '"';
                                break;
                            case '\\':
                                value += '\\';
                                break;
                            case 'n':
                                value += '\n';
                                break;
                            case 't':
                                value += '\t';
                                break;
                            case 'r':
                                value += '\r';
                                break;
                            case 'b':
                                value += '\b';
                                break;
                            case 'f':
                                value += '\f';
                                break;
                            case '/':
                                value += '/';
                                break;
                            default:
                                return Token{TokenType::Error,
                                             std::format("invalid escape '\\{}' at line {}, column {}",
                                                         current(), line_, column_),
                                             start_line, start_column};
                        }
                    } else {
                        value += current();
                    }
                    advance();
                }

                if (current() != '"') {
                    return Token{TokenType::Error,
                                 std::format("unterminated string starting at line {}, column {}",
                                             start_line, start_column),
                                 start_line, start_column};
                }
                advance();

                return Token{TokenType::String, value, start_line, start_column};
            }
        }

        if (std::isdigit(static_cast<unsigned char>(c)) || c == '-') {
            std::string lexeme;

            if (current() == '-') {
                lexeme += '-';
                advance();
            }

            if (!std::isdigit(static_cast<unsigned char>(current()))) {
                return Token{TokenType::Error,
                             std::format("expected digit after '-' at line {}, column {}", start_line, start_column),
                             start_line, start_column};
            }

            while (std::isdigit(static_cast<unsigned char>(current()))) {
                lexeme += current();
                advance();
            }

            if (current() == '.') {
                lexeme += '.';
                advance();
                while (std::isdigit(static_cast<unsigned char>(current()))) {
                    lexeme += current();
                    advance();
                }
            }

            if (current() == 'e' || current() == 'E') {
                lexeme += current();
                advance();
                if (current() == '+' || current() == '-') {
                    lexeme += current();
                    advance();
                }
                if (!std::isdigit(static_cast<unsigned char>(current()))) {
                    return Token{TokenType::Error,
                                 std::format("expected digit in exponent at line {}, column {}",
                                             start_line, start_column),
                                 start_line, start_column};
                }
                while (std::isdigit(static_cast<unsigned char>(current()))) {
                    lexeme += current();
                    advance();
                }
            }

            return Token{TokenType::Number, lexeme, start_line, start_column};
        }

        if (std::isalpha(static_cast<unsigned char>(c))) {
            std::string lexeme;
            while (std::isalpha(static_cast<unsigned char>(current()))) {
                lexeme += current();
                advance();
            }

            if (lexeme == "true")
                return Token{TokenType::True, lexeme, start_line, start_column};
            if (lexeme == "false")
                return Token{TokenType::False, lexeme, start_line, start_column};
            if (lexeme == "null")
                return Token{TokenType::Null, lexeme, start_line, start_column};

            return Token{TokenType::Error,
                         std::format("unknown keyword '{}' at line {}, column {}", lexeme, start_line, start_column),
                         start_line, start_column};
        }

        advance();
        return Token{TokenType::Error,
                     std::format("unexpected character '{}' at line {}, column {}", c, start_line, start_column), start_line, start_column};
    }
} // namespace hp::ser