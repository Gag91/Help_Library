#pragma once

#include "hp/concepts/concept.hpp"
#include "hp/string/string.hpp"

namespace hp::parse {

    // ----- Parse single block: e.g. "{42}" -> 42, "[MyEnum]" -> MyEnum::Value
    template <typename T>
    [[nodiscard]] inline T block(char first, char last, std::string_view value) {
        const std::size_t pos1 = value.find(first);
        if (pos1 == std::string_view::npos)
            throw std::runtime_error(std::format("hp::parse::block: '{}' not found", first));

        const std::size_t pos2 = value.find(last, pos1 + 1);
        if (pos2 == std::string_view::npos)
            throw std::runtime_error(std::format("hp::parse::block: '{}' not found", last));

        const std::string_view inner = hp::str::trim_view(value.substr(pos1 + 1, pos2 - (pos1 + 1)));
        return hp::str::from_string<T>(std::string(inner));
    }

    // ----- Parse container block: e.g. "(a, b, c)" -> std::vector<T>
    template <hp::Container Container>
        requires(!std::same_as<std::remove_cvref_t<Container>, std::string> && !std::same_as<std::remove_cvref_t<Container>, std::string_view>)
    [[nodiscard]] inline Container block(char first, char last, char symbol, std::string_view value) {
        using Elem = typename std::remove_cvref_t<Container>::value_type;

        const std::size_t pos1 = value.find(first);
        if (pos1 == std::string_view::npos)
            throw std::runtime_error(std::format("hp::parse::block: '{}' not found", first));

        const std::size_t pos2 = value.find(last, pos1 + 1);
        if (pos2 == std::string_view::npos)
            throw std::runtime_error(std::format("hp::parse::block: '{}' not found", last));

        const std::string_view inner = value.substr(pos1 + 1, pos2 - (pos1 + 1));

        Container result{};
        std::size_t idx = 0;

        for (const auto &part : inner | std::views::split(symbol)) {
            const std::string_view item = hp::str::trim_view(std::string_view(part));
            if (item.empty())
                continue;

            Elem v = hp::str::from_string<Elem>(hp::str::unquote(item));

            if constexpr (requires { result.push_back(v); }) {
                result.push_back(std::move(v));
            } else if constexpr (requires { result.insert(v); }) {
                result.insert(std::move(v));
            } else if constexpr (requires { result[idx] = v; }) {
                result[idx++] = std::move(v);
            }
        }
        return result;
    }

    // ----- Extract balanced nested block: e.g. "{ a, { b, c }, d }"
    [[nodiscard]] inline std::string_view block(char open, char close, std::string_view text) {
        const std::size_t start = text.find(open);
        if (start == std::string_view::npos)
            throw std::runtime_error(std::format("hp::parse::block: '{}' not found", open));

        int depth = 0;
        bool in_quotes = false;
        char quote_char = '\0';

        for (std::size_t i = start; i < text.size(); ++i) {
            const char c = text[i];

            if (in_quotes) {
                if (c == '\\' && i + 1 < text.size()) {
                    ++i;
                } else if (c == quote_char) {
                    in_quotes = false;
                }
                continue;
            }

            if (c == '"' || c == '\'') {
                in_quotes = true;
                quote_char = c;
            } else if (c == open) {
                ++depth;
            } else if (c == close) {
                if (--depth == 0) {
                    return hp::str::trim_view(text.substr(start + 1, i - (start + 1)));
                }
            }
        }
        throw std::runtime_error(std::format("hp::parse::block: unmatched '{}'", open));
    }

    // ----- Split quotes & nested brackets: e.g. R"(a, "b, c", [1, 2])"
    [[nodiscard]] inline std::vector<std::string_view> split_smart(std::string_view text, char delimiter = ',') {
        std::vector<std::string_view> out;
        std::size_t start = 0;
        int depth = 0;
        bool in_quotes = false;
        char quote_char = '\0';

        for (std::size_t i = 0; i <= text.size(); ++i) {
            if (i < text.size()) {
                const char c = text[i];
                if (in_quotes) {
                    if (c == '\\' && i + 1 < text.size()) {
                        ++i;
                    } else if (c == quote_char) {
                        in_quotes = false;
                    }
                    continue;
                }

                if (c == '"' || c == '\'') {
                    in_quotes = true;
                    quote_char = c;
                } else if (c == '[' || c == '(' || c == '{') {
                    ++depth;
                } else if (c == ']' || c == ')' || c == '}') {
                    --depth;
                }
            }

            if (i == text.size() || (text[i] == delimiter && depth == 0 && !in_quotes)) {
                const std::string_view piece = hp::str::trim_view(text.substr(start, i - start));
                if (!piece.empty()) {
                    out.push_back(piece);
                }
                start = i + 1;
            }
        }
        return out;
    }

    // ----- Key-Value pair: e.g. "port = 8080" -> std::pair<std::string, int>
    template <typename K = std::string, typename V = std::string>
    [[nodiscard]] inline std::pair<K, V> key_value(std::string_view line, char sep = '=') {
        std::size_t pos = line.find(sep);
        if (pos == std::string_view::npos)
            throw std::runtime_error(std::format("hp::parse::key_value: separator '{}' not found", sep));

        std::string_view raw_k = hp::str::trim_view(line.substr(0, pos));
        std::string_view raw_v = hp::str::trim_view(line.substr(pos + 1));

        return {
            hp::str::from_string<K>(hp::str::unquote(raw_k)),
            hp::str::from_string<V>(hp::str::unquote(raw_v)),
        };
    }

    [[nodiscard]] inline std::string_view strip_comment(std::string_view line, std::string_view prefix = "#") {
        bool in_quotes = false;
        char quote_char = '\0';

        for (std::size_t i = 0; i < line.size(); ++i) {
            const char c = line[i];
            if (in_quotes) {
                if (c == '\\' && i + 1 < line.size()) {
                    ++i;
                } else if (c == quote_char) {
                    in_quotes = false;
                }
                continue;
            }

            if (c == '"' || c == '\'') {
                in_quotes = true;
                quote_char = c;
            } else if (line.substr(i).starts_with(prefix)) {
                return hp::str::trim_view(line.substr(0, i));
            }
        }
        return hp::str::trim_view(line);
    }

    // ----- Expand ${VAR} placeholders
    template <typename Lookup>
    [[nodiscard]] inline std::string expand_vars(std::string_view text, Lookup &&lookup) {
        std::string out;
        out.reserve(text.size());

        std::size_t pos = 0;
        while (pos < text.size()) {
            std::size_t start = text.find("${", pos);
            if (start == std::string_view::npos) {
                out.append(text.substr(pos));
                break;
            }

            std::size_t end = text.find('}', start + 2);
            if (end == std::string_view::npos) {
                out.append(text.substr(pos));
                break;
            }

            out.append(text.substr(pos, start - pos));
            const std::string key(hp::str::trim_view(text.substr(start + 2, end - (start + 2))));

            if constexpr (requires { lookup(key); }) {
                out.append(lookup(key));
            } else if constexpr (requires { lookup.at(key); }) {
                if (auto it = lookup.find(key); it != lookup.end()) {
                    out.append(hp::str::to_string(it->second));
                }
            }
            pos = end + 1;
        }
        return out;
    }

    // ----- Suggest closest match
    template <std::ranges::input_range Candidates>
    [[nodiscard]] inline std::optional<std::string> suggest(std::string_view input, const Candidates &candidates) {
        for (const auto &cand : candidates) {
            if (hp::str::is_typo(input, cand)) {
                return hp::str::to_string(cand);
            }
        }
        return std::nullopt;
    }

} // namespace hp::parse