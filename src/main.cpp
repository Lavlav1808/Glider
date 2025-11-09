#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

#include "./tokenization.hpp"

std::string token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::exitcode:
            return "exitcode";
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
    for (size_t i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::exitcode) {
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

    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();

    std::cout << tokens_to_asm(tokens) << std::endl;

    {
        std::fstream file("out.asm", std::ios_base::out);
        file << tokens_to_asm(tokens);
    }

    if (system("nasm -felf64 out.asm") != 0) {
        std::cerr << "Error: nasm command failed." << std::endl;
        return EXIT_FAILURE;
    }
    if (system("ld -o out out.o") != 0) {
        std::cerr << "Error: ld command failed." << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
