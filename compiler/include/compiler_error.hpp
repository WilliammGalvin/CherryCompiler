#ifndef COMPILER_ERROR_HPP
#define COMPILER_ERROR_HPP

#include <exception>
#include <string>

namespace compiler {

    class CompilerError : std::exception {
        std::string message;

        mutable std::string formatted_msg;

    public:
        CompilerError(const std::string& msg);

        const char* what() const noexcept override;
    };

}

#endif //COMPILER_ERROR_HPP
