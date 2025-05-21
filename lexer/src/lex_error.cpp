#include "../include/lex_error.hpp"

namespace lexer {

    LexError::LexError(const std::string& msg, const int line, const int column) {
        this->message = msg;
        this->line = line;
        this->column = column;
    }

    const char* LexError::what() const noexcept {
        formatted_msg = "Error encountered on line " + std::to_string(line) +
            ", column " + std::to_string(column) + ":\n" + message;

        return formatted_msg.c_str();
    }


}
