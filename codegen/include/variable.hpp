//
// Created by Willam Galvin on 2025-05-18.
//

#ifndef VARIABLE_HPP
#define VARIABLE_HPP
#include "../../parser/include/ast_var_types.hpp"

namespace codegen {

    struct Variable {
        parser::ASTValueType type;
        bool muttable;

        Variable(parser::ASTValueType type, bool muttable);
    };

}

#endif //VARIABLE_HPP
