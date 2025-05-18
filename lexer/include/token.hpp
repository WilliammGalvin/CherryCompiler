//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>

namespace lexer {

    enum TokenType {
        BUILTIN_FUNC,
        LINE_END,
        STRING_LITERAL,
        NUMBER,
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        LEFT_PAREN,
        RIGHT_PAREN,
        EQUALS,
        IDENTIFIER,
        KEYWORD
    };

    struct Token {
        TokenType type;
        std::string value;

        Token(TokenType type, const std::string& value);

        Token(TokenType type);

        std::string to_str() const;
    };

}

#endif //TOKEN_HPP
