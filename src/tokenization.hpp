#pragma once
#include <vector>
#include <string>
#include <optional>
#include <iostream>

enum class TokenType { exitcode, int_literal, semi };

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

class Tokenizer {
public:
    inline explicit Tokenizer(std::string src)
        : m_src(std::move(src)), m_index(0) {
    }

    inline std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buffer;
        while (peak().has_value()) {
            if (std::isspace(peak().value())) {
                consume();
                continue;
            }
            if (std::isalpha(peak().value())) {
                buffer.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value())) {
                    buffer.push_back(consume());
                }
                if (buffer == "exitcode") {
                    tokens.push_back({.type = TokenType::exitcode});
                    buffer.clear();
                } else {
                    std::cerr << "Unexpected Token Type: " << buffer << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if (std::isdigit(peak().value())) {
                buffer.push_back(consume());
                while (peak().has_value() && std::isdigit(peak().value())) {
                    buffer.push_back(consume());
                }
                tokens.push_back({.type = TokenType::int_literal, .value = buffer});
                buffer.clear();
            }
            else if (peak().value() == ';') {
                consume();
                tokens.push_back({.type = TokenType::semi});
            }
            else {
                std::cerr << "Unexpected token type: " << peak().value() << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }

private:
    [[nodiscard]] std::optional<char> peak(int ahead = 0) const {
        if (m_index + ahead > m_src.length()) {
            return {};
        } else {
            return m_src.at(m_index + ahead);
        }
    }

    char consume() {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    size_t m_index;
};