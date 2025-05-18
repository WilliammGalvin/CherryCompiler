//
// Created by Willam Galvin on 2025-05-18.
//

#include "../include/ast_var_types.hpp"

namespace parser {

    std::string ast_val_type_str(ASTValueType val) {
        switch (val) {
            case STRING_LITERAL: return "STRING_LITERAL";
            case FLOAT: return "FLOAT";
            case INTEGER: return "INTEGER";
        }
    }

    ASTValueType get_var_type_from_node(ASTNode* node) {
        if (dynamic_cast<StringLiteral*>(node)) {
            return STRING_LITERAL;
        }

        if (dynamic_cast<Float*>(node)) {
            return FLOAT;
        }

        if (dynamic_cast<Integer*>(node)) {
            return INTEGER;
        }
    }

}