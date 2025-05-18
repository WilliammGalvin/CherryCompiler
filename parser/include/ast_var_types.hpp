//
// Created by Willam Galvin on 2025-05-18.
//

#ifndef AST_UTIL_HPP
#define AST_UTIL_HPP
#include "ast_nodes.hpp"

namespace parser {

    enum ASTValueType {
        STRING_LITERAL,
        FLOAT,
        INTEGER,
    };

    std::string ast_val_type_str(ASTValueType val);

    ASTValueType get_var_type_from_node(ASTNode* node);

}

#endif //AST_UTIL_HPP
