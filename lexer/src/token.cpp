//
// Created by Willam Galvin on 2025-05-17.
//

#include <cassert>

#include "../include/token.hpp"

namespace lexer {

    Token::Token(const TokenType type, const std::string &value) {
        this->type = type;
        this->value = value;
    }

    Token::Token(const TokenType type) {
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
            case ADD: str = "ADD"; break;
            case SUBTRACT: str = "SUBTRACT"; break;
            case MULTIPLY: str = "MULTIPLY"; break;
            case DIVIDE: str = "DIVIDE"; break;
            case LEFT_PAREN: str = "LEFT_PAREN"; break;
            case RIGHT_PAREN: str = "RIGHT_PAREN"; break;
            case EQUALS: str = "EQUALS"; break;
            case IDENTIFIER: str = "IDENTIFIER"; break;
            case KEYWORD: str = "KEYWORD"; break;
            default:
                assert(false && "Can't find str match for token enum.");
        }

        return std::string(str + "(" + value + ")");
    }


}
