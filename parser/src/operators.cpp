#include <cassert>

#include "../include/operators.hpp"

namespace parser {

    bool is_binary_op(const lexer::TokenType type) {
        switch (type) {
            case lexer::ADD:
            case lexer::SUBTRACT:
            case lexer::MULTIPLY:
            case lexer::DIVIDE:
                return true;

            default:
                return false;
        }
    }

    std::string binary_operator_to_str(const BinaryOperator op) {
        switch (op) {
            case ADD: return "ADD";
            case SUBTRACT: return "SUBTRACT";
            case MULTIPLY: return "MULTIPLY";
            case DIVIDE: return "DIVIDE";
        }

        assert(false && "Can't find str match for BinaryOperator enum");
        return "";
    }

}
