//
// Created by Willam Galvin on 2025-05-18.
//

#include "../include/operators.hpp"

namespace parser {

    std::string binary_operator_to_str(BinaryOperator op) {
        switch (op) {
            case ADD: return "ADD";
            case SUBTRACT: return "SUBTRACT";
            case MULTIPLY: return "MULTIPLY";
            case DIVIDE: return "DIVIDE";
        }
    }

}
