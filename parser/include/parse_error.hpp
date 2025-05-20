//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef PARSE_ERROR_HPP
#define PARSE_ERROR_HPP

#include <exception>
#include <string>

namespace parser {

    class ParseError final : public std::exception {
        std::string message;

        mutable std::string formatted_msg;

    public:
        explicit ParseError(const std::string& msg);

        const char* what() const noexcept override;
    };

}

#endif //PARSE_ERROR_HPP
