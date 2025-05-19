//
// Created by Willam Galvin on 2025-05-18.
//

#ifndef VARIABLE_HPP
#define VARIABLE_HPP
#include <unordered_map>
#include <utility>
#include <variant>

#include "../../parser/include/ast_nodes.hpp"

namespace codegen {

    using ValueVariant = std::variant<std::string, float, int>;

    struct Variable {
        parser::ASTValueType type;
        ValueVariant value;
        bool muttable;

        Variable() = default;

        Variable(parser::ASTValueType type, ValueVariant value, bool muttable) {
            this->type = type;
            this->value = std::move(value);
            this->muttable = muttable;
        }
    };

    using VariableMap = std::unordered_map<std::string, Variable>;

}

#endif //VARIABLE_HPP
