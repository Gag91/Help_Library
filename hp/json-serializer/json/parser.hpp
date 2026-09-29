#pragma once

#include "lexer.hpp"

#include <cstddef>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace hp::ser {

    struct JsonValue;
    using JsonValuePtr = std::shared_ptr<JsonValue>;

    std::string to_string(const JsonValue &value);
    std::string to_string(const JsonValuePtr &value);

    struct JsonValue {
        std::variant<
            std::nullptr_t,
            bool,
            double,
            std::string,
            std::vector<JsonValuePtr>,
            std::map<std::string, JsonValuePtr>>
            data;
    };

    class Parser {
      public:
        explicit Parser(std::string_view source) : lexer_(source) {
            advance();
        }
        JsonValuePtr parse();

      private:
        Lexer lexer_;
        Token current_;

        void advance();
        JsonValuePtr parse_value();
        JsonValuePtr parse_object();
        JsonValuePtr parse_array();
    };

    inline void Parser::advance() {
        current_ = lexer_.next();
    }

    inline JsonValuePtr Parser::parse() {
        return parse_value();
    }

    inline JsonValuePtr Parser::parse_value() {
        switch (current_.type) {
            case TokenType::LeftBrace:
                return parse_object();
            case TokenType::LeftBracket:
                return parse_array();
            case TokenType::String: {
                auto v = std::make_shared<JsonValue>();
                v->data = current_.text;
                advance();
                return v;
            }
            case TokenType::Number: {
                auto v = std::make_shared<JsonValue>();
                v->data = std::stod(current_.text);
                advance();
                return v;
            }
            case TokenType::True: {
                auto v = std::make_shared<JsonValue>();
                v->data = true;
                advance();
                return v;
            }
            case TokenType::False: {
                auto v = std::make_shared<JsonValue>();
                v->data = false;
                advance();
                return v;
            }
            case TokenType::Null: {
                auto v = std::make_shared<JsonValue>();
                v->data = nullptr;
                advance();
                return v;
            }
            case TokenType::Error:
                throw std::runtime_error(current_.text);
            case TokenType::EndOfFile:
                throw std::runtime_error("unexpected end of input");
            default:
                throw std::runtime_error("unexpected token");
        }
    }

    inline JsonValuePtr Parser::parse_object() {
        std::map<std::string, JsonValuePtr> obj;
        advance();

        if (current_.type == TokenType::RightBrace) {
            advance();
            auto v = std::make_shared<JsonValue>();
            v->data = std::move(obj);
            return v;
        }

        while (true) {
            if (current_.type != TokenType::String) {
                throw std::runtime_error("expected string key in object");
            }
            std::string key = current_.text;
            advance();

            if (current_.type != TokenType::Colon) {
                throw std::runtime_error("expected ':' after object key");
            }
            advance();

            obj[key] = parse_value();

            if (current_.type == TokenType::RightBrace) {
                advance();
                auto v = std::make_shared<JsonValue>();
                v->data = std::move(obj);
                return v;
            }
            if (current_.type != TokenType::Comma) {
                throw std::runtime_error("expected ',' or '}' in object");
            }
            advance();
        }
    }

    inline JsonValuePtr Parser::parse_array() {
        std::vector<JsonValuePtr> arr;
        advance();

        if (current_.type == TokenType::RightBracket) {
            advance();
            auto v = std::make_shared<JsonValue>();
            v->data = std::move(arr);
            return v;
        }

        while (true) {
            arr.push_back(parse_value());

            if (current_.type == TokenType::RightBracket) {
                advance();
                auto v = std::make_shared<JsonValue>();
                v->data = std::move(arr);
                return v;
            }

            if (current_.type != TokenType::Comma) {
                throw std::runtime_error("expected ',' or ']' in array");
            }
            advance();
        }
    }

    std::string to_string(const JsonValue &value) {
        if (std::holds_alternative<std::nullptr_t>(value.data)) {
            return "null";
        }

        if (std::holds_alternative<bool>(value.data)) {
            return std::get<bool>(value.data) ? "true" : "false";
        }

        if (std::holds_alternative<double>(value.data)) {
            return std::format("{}", std::get<double>(value.data));
        }

        if (std::holds_alternative<std::string>(value.data)) {
            return "\"" + std::get<std::string>(value.data) + "\"";
        }

        if (std::holds_alternative<std::vector<JsonValuePtr>>(value.data)) {
            auto &arr = std::get<std::vector<JsonValuePtr>>(value.data);
            std::string result = "[";
            bool first = true;

            for (const auto &a : arr) {
                if (!first)
                    result += ", ";
                first = false;
                result += to_string(*a);
            }
            return result += "]";
        }

        if (std::holds_alternative<std::map<std::string, JsonValuePtr>>(value.data)) {
            auto &vec = std::get<std::map<std::string, JsonValuePtr>>(value.data);
            std::string result = "{";
            bool first = true;

            for (const auto [key, value] : vec) {
                if (!first)
                    result += ", ";
                first = false;
                result += "\"" + key + "\": " + to_string(*value);
            }
            return result += "}";
        }
        return "unknown";
    }

    std::string to_string(const JsonValuePtr &value) {
        return to_string(*value);
    }
} // namespace hp::ser