#include <fstream>
#include <iostream>

#include "codegen/include/code_gen_error.hpp"
#include "codegen/include/c_gen.hpp"
#include "lexer/include/lexer.hpp"
#include "lexer/include/lex_error.hpp"
#include "parser/include/parser.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: Expected exactly one argument." << std::endl;
        std::cerr << "Usage: " << argv[0] << " [launch_file].ch" << std::endl;
        return 1;
    }

    const std::string launch_path = argv[1];

    if (!launch_path.ends_with(".ch")) {
        std::cerr << "Error: Expected launch file with .ch extension." << std::endl;
        return 1;
    }

    lexer::Lexer lexer(launch_path);
    std::vector<lexer::Token> tokens{};

    try {
        tokens = lexer.lex_file();
    } catch (const lexer::LexError& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }

    for (const auto& token : tokens) {
        std::cout << token.to_str() << std::endl;
    }

    std::cout << "~~~~~~" << std::endl;

    auto asts = parser::build_program(tokens);

    for (const auto& ast : asts) {
        ast->print(std::cout, 0);
    }

    std::cout << "~~~~~~" << std::endl;

    codegen::CGen gen;

    try {
        std::ofstream file("output/test.c");
        if (!file.is_open()) {
            std::cerr << "Failed to open output file." << std::endl;
            return 1;
        }

        gen.generate(asts, file);
    } catch (const codegen::CodeGenError& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }

    std::cout << "File compiled." << std::endl;
    asts.clear();
    return 0;
}
