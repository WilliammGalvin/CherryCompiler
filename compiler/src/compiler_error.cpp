#include "../include/compiler_error.hpp"

namespace compiler {

    CompilerError::CompilerError(const std::string& msg) {
        this->message = msg;
    }

    const char* CompilerError::what() const noexcept {
        formatted_msg = "Error: " + message;
        return formatted_msg.c_str();
    }

}