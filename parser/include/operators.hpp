#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include <string>

#include "../../lexer/include/token.hpp"

namespace parser {

    enum BinaryOperator {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE
    };

    bool is_binary_op(lexer::TokenType type);

    std::string binary_operator_to_str(BinaryOperator op);

}

#endif //OPERATORS_HPP
