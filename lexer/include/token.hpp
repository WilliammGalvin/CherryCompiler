#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace lexer {

    enum TokenType {
        BUILTIN_FUNC,
        LINE_END,
        STRING_LITERAL,
        FLOAT,
        INTEGER,
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

        explicit Token(TokenType type);

        [[nodiscard]] std::string to_str() const;
    };

}

#endif //TOKEN_HPP
