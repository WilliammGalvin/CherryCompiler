//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef CODE_GEN_ERROR_HPP
#define CODE_GEN_ERROR_HPP
#include <exception>
#include <string>

namespace codegen {

    class CodeGenError : std::exception {
        std::string message;

        mutable std::string formatted_msg;

    public:
        CodeGenError(const std::string& msg);

        const char* what() const noexcept override;
    };

}

#endif //CODE_GEN_ERROR_HPP
