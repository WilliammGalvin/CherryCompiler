#include "../include/parse_error.hpp"

namespace parser {

    ParseError::ParseError(const std::string& msg) {
        this->message = msg;
    }

    const char* ParseError::what() const noexcept {
        formatted_msg = "Error: " + message;
        return formatted_msg.c_str();
    }

}