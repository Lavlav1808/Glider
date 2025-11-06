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
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string& str) {};

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

    std::cout << contents << std::endl;

    return EXIT_SUCCESS;
}