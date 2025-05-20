//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef LEXERROR_HPP
#define LEXERROR_HPP

#include <exception>
#include <string>

namespace lexer {

    class LexError final : public std::exception {
        std::string message;
        int line;
        int column;

        mutable std::string formatted_msg;

    public:
        LexError(const std::string& msg, int line, int column);

        const char* what() const noexcept override;
    };

}

#endif //LEXERROR_HPP
