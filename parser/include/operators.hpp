//
// Created by Willam Galvin on 2025-05-18.
//

#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include <string>

namespace parser {

    enum BinaryOperator {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE
    };

    std::string binary_operator_to_str(BinaryOperator op);

}

#endif //OPERATORS_HPP
