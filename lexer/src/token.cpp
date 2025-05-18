//
// Created by Willam Galvin on 2025-05-17.
//

#include "../include/token.hpp"

#include <assert.h>
#include <iostream>

namespace lexer {

    Token::Token(TokenType type, const std::string &value) {
        this->type = type;
        this->value = value;
    }

    Token::Token(TokenType type) {
        this->type = type;
        this->value = {};
    }

    std::string Token::to_str() const {
        std::string str{};

        switch (type) {
            case BUILTIN_FUNC: str = "BUILTIN_FUNC"; break;
            case STRING_LITERAL: str = "STRING_LITERAL"; break;
            case NUMBER: str = "NUMBER"; break;
            case LINE_END: str = "LINE_END"; break;
            default:
                assert(false && "Can't find str match for token enum.");
        }

        return std::string(str + "(" + value + ")");
    }


}
