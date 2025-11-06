#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

enum class TokenType {
    _return,
    int_literal,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens;

    std::string buffer;
    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        if (std::isalpha(c)) {
            buffer.push_back(c);
            i++;
            while (std::isalnum(str.at(i))) {
                buffer.push_back(str.at(i));
                i++;
            }
            i--;

            if (buffer == "return") {
                tokens.push_back({.type = TokenType::_return});
                buffer.clear();
                continue;
            } else {
                std::cerr << "Unexpected token type: " << buffer << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (isdigit(c)) {
            buffer.push_back(c);
            i++;
            while (isdigit(str.at(i))) {
                buffer.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type = TokenType::int_literal, .value = buffer});
            buffer.clear();
        }
        else if (c == ';') {
            tokens.push_back({.type = TokenType::semi});
        }
        else if (std::isspace(c)) {
            continue;
        }
        else {
            std::cerr << "Unexpected token type: " << buffer << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return  tokens;
};

std::string token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::_return:
            return "_return";
        case TokenType::int_literal:
            return "int_literal";
        case TokenType::semi:
            return "semi";
    }
    return "unknown";
}

std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;
    output << "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::_return) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_literal) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }
    }
    return output.str();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect Usage. Correct usage:" << std::endl;
        std::cerr << "Glider <input.gld>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream content_stream;
        std::fstream input(argv[1], std::ios_base::in);
        content_stream << input.rdbuf();
        contents = content_stream.str();
    }

    std::vector<Token> tokens = tokenize(contents);

    for (const auto& token : tokens) {
        std::cout << "Token { type: " << token_type_to_string(token.type);
        if (token.value.has_value()) {
            std::cout << ", value: '" << token.value.value() << "'";
        }
        std::cout << " }" << std::endl;
    }

    std::cout << tokens_to_asm(tokens) << std::endl;

    return EXIT_SUCCESS;
}