//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

#include "token.hpp"

namespace lexer {

    class Lexer {
        std::string file_name;
        std::string current_source;
        int line;
        int index;

        char peek();
        char consume();

        bool match_symbol(std::vector<Token>& tokens);
        bool match_builtin_func(std::vector<Token>& tokens);
        bool match_keyword(std::vector<Token>& tokens);
        bool match_identifier(std::vector<Token>& tokens);
        bool match_string_literal(std::vector<Token>& tokens);
        bool match_number(std::vector<Token>& tokens);

        void lex_line(std::vector<Token>& tokens);

    public:
        explicit Lexer(const std::string& file_name);

        std::vector<Token> lex_file();
    };

}

#endif //LEXER_HPP
