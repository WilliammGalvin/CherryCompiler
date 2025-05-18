//
// Created by Willam Galvin on 2025-05-18.
//

#include "../include/variable.hpp"

namespace codegen {

    Variable::Variable(parser::ASTValueType type, bool muttable) {
        this->type = type;
        this->muttable = muttable;
    }

}
