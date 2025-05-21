#include "../include/code_gen_error.hpp"

namespace codegen {

    CodeGenError::CodeGenError(const std::string &msg) {
        this->message = msg;
    }

    const char *CodeGenError::what() const noexcept {
        formatted_msg = "Error: " + message;
        return formatted_msg.c_str();
    }

}