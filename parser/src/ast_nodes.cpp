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
            case BUILTIN_FUNC: return "BUILTIN_FUNC";
            case IMM_DECLARE: return "IMM_DECLARE";
            case ASSIGN_VAR: return "ASSIGN_VAR";
            case BINARY_OP: return "BINARY_OP";
            default: {
                throw ParseError("Couldn't map ASTValueType enum to str.");
            };
        }
    }

    BuiltInFunc::BuiltInFunc(const std::string& func_name, std::unique_ptr<ASTNode> arg) {
        this->func_name = func_name;
        this->arg = std::move(arg);
        this->type = BUILTIN_FUNC;
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
        this->type = STRING_LITERAL;
    }

    void StringLiteral::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "StringLiteral: \"" << content << "\"\n";
    }

    Integer::Integer(const int value) {
        this->value = value;
        this->type = INTEGER;
    }

    void Integer::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "Integer: " << value << "\n";
    }

    Float::Float(const float value) {
        this->value = value;
        this->type = FLOAT;
    }

    void Float::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "Float: " << value << "\n";
    }

    Identifier::Identifier(const std::string& name) {
        this->name = name;
        this->type = IDENTIFIER;
    }

    void Identifier::print(std::ostream& os, const int indent_level) const {
        indent(os, indent_level);
        os << "Identifier: " << name << "\n";
    }

    ImmDeclare::ImmDeclare(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value) {
        this->identifier = std::move(identifier);
        this->value = std::move(value);
        this->type = IMM_DECLARE;
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

    MutDeclare::MutDeclare(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value) {
        this->identifier = std::move(identifier);
        this->value = std::move(value);
        this->type = MUT_DECLARE;
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

    AssignVar::AssignVar(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value) {
        this->identifier = std::move(identifier);
        this->value = std::move(value);
        this->type = ASSIGN_VAR;
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
        const BinaryOperator op
    ) {
        this->left = std::move(left);
        this->right = std::move(right);
        this->op = op;
        this->type = BINARY_OP;
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

        indent(os, indent_level + 1);
        os << "Left Type: " << ast_val_type_str(left->type) << "\n";
        left->print(os, indent_level + 1);

        indent(os, indent_level + 1);
        os << "Right Type: " << ast_val_type_str(left->type) << "\n";
        right->print(os, indent_level + 1);
    }

}
