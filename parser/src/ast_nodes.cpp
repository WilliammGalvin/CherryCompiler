//
// Created by Willam Galvin on 2025-05-17.
//

#include <cassert>

#include "../include/ast_nodes.hpp"
#include "../include/parse_error.hpp"

namespace parser {

    static void indent(std::ostream& os, const int level) {
        for (int i = 0; i < level; ++i) os << "  ";
    }

    std::string ast_val_type_str(const ASTValueType val) {
        switch (val) {
            case STRING_LITERAL: return "STRING_LITERAL";
            case FLOAT: return "FLOAT";
            case INTEGER: return "INTEGER";
            case IDENTIFIER: return "IDENTIFIER";
        }

        assert(false && "Cannot identify string for ASTValueType.");
        return "";
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

        if (dynamic_cast<Identifier*>(node)) {
            return IDENTIFIER;
        }

        assert(false && "Cannot determine var type from ASTNode.");
        return INTEGER;
    }

    BuiltInFunc::BuiltInFunc(
        const std::string& func_name,
        std::unique_ptr<ASTNode> arg
        ) {
        this->func_name = func_name;
        this->arg = std::move(arg);
    }

    void BuiltInFunc::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "BuiltInFunc: " << func_name << "\n";

        if (arg) {
            arg->print(os, indent_level + 1);
        } else {
            indent(os, indent_level + 1);
            os << "Arg value is null\n";
        }
    }

    StringLiteral::StringLiteral(const std::string& content) {
        this->content = content;
    }

    void StringLiteral::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "StringLiteral: \"" << content << "\"\n";
    }

    Integer::Integer(const int value) {
        this->value = value;
    }

    void Integer::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "Integer: " << value << "\n";
    }

    Float::Float(const float value) {
        this->value = value;
    }

    void Float::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "Float: " << value << "\n";
    }

    Identifier::Identifier(const std::string& name) {
        this->name = name;
    }

    void Identifier::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "Identifier: " << name << "\n";
    }

    ImmDeclare::ImmDeclare(
        std::unique_ptr<ASTNode> identifier,
        std::unique_ptr<ASTNode> value
    ) {
        this->identifier = std::move(identifier);
        this->value = std::move(value);
    }

    void ImmDeclare::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "ImmDeclare:\n";

        if (identifier) {
            identifier->print(os, indent_level + 1);
        } else {
            indent(os, indent_level + 1);
            os << "Identifier value is null\n";
        }

        if (value) {
            value->print(os, indent_level + 1);
        } else {
            indent(os, indent_level + 1);
            os << "Value is null\n";
        }
    }

    MutDeclare::MutDeclare(
        std::unique_ptr<ASTNode> identifier,
        std::unique_ptr<ASTNode> value
    ) {
        this->identifier = std::move(identifier);
        this->value = std::move(value);
    }

    void MutDeclare::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "MutDeclare:\n";

        if (identifier) {
            identifier->print(os, indent_level + 1);
        } else {
            indent(os, indent_level + 1);
            os << "Identifier value is null\n";
        }

        if (value) {
            value->print(os, indent_level + 1);
        } else {
            indent(os, indent_level + 1);
            os << "Value is null\n";
        }
    }

    AssignVar::AssignVar(
        std::unique_ptr<ASTNode> identifier,
        std::unique_ptr<ASTNode> value
    ) {
        this->identifier = std::move(identifier);
        this->value = std::move(value);
    }

    void AssignVar::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "AssignVar:\n";

        if (identifier) {
            identifier->print(os, indent_level + 1);
        } else {
            indent(os, indent_level + 1);
            os << "Identifier value is null\n";
        }

        if (value) {
            value->print(os, indent_level + 1);
        } else {
            indent(os, indent_level + 1);
            os << "Value is null\n";
        }
    }

    BinaryOp::BinaryOp(
        std::unique_ptr<ASTNode> left,
        std::unique_ptr<ASTNode> right,
        const BinaryOperator op,
        const ASTValueType left_type,
        const ASTValueType right_type
    ) {
        this->left = std::move(left);
        this->right = std::move(right);
        this->op = op;
        this->left_type = left_type;
        this->right_type = right_type;
    }

    void BinaryOp::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "BinaryOp:\n";

        if (op) {
            indent(os, indent_level + 1);
            os << "Operator: " << binary_operator_to_str(op) << "\n";
        } else {
            indent(os, indent_level + 1);
            os << "Operator is null\n";
        }

        if (left_type) {
            indent(os, indent_level + 1);
            os << "Left Type: " << ast_val_type_str(left_type) << "\n";
            left->print(os, indent_level + 1);
        } else {
            indent(os, indent_level + 1);
            os << "Left type is null\n";
        }

        if (right_type) {
            indent(os, indent_level + 1);
            os << "Right Type: " << ast_val_type_str(right_type) << "\n";
            right->print(os, indent_level + 1);
        } else {
            indent(os, indent_level + 1);
            os << "Right type is null\n";
        }
    }

}
