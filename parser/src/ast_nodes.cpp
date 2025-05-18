//
// Created by Willam Galvin on 2025-05-17.
//

#include "../include/ast_nodes.hpp"
#include "../include/parse_error.hpp"

namespace parser {

    static void indent(std::ostream& os, int level) {
        for (int i = 0; i < level; ++i) os << "  ";
    }

    BuiltInFunc::BuiltInFunc(const std::string& func_name, std::unique_ptr<ASTNode> arg) {
        this->func_name = func_name;
        this->arg = std::move(arg);
    }

    void BuiltInFunc::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "BuiltInFunc: " << func_name << "\n";

        if (arg) {
            arg->print(os, indent_level + 1);
        }
    }

    StringLiteral::StringLiteral(const std::string& content) {
        this->content = content;
    }

    void StringLiteral::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "StringLiteral: \"" << content << "\"\n";
    }

    Integer::Integer(int value) {
        this->value = value;
    }

    void Integer::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "Integer: " << value << "\n";
    }

    Float::Float(float value) {
        this->value = value;
    }

    void Float::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "Float: " << value << "\n";
    }

    Identifier::Identifier(const std::string& name) {
        this->name = name;
    }

    void Identifier::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "Identifier: " << name << "\n";
    }

    ImmDeclare::ImmDeclare(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value) {
        this->identifier = std::move(identifier);
        this->value = std::move(value);
    }

    void ImmDeclare::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "ImmDeclare:\n";

        if (identifier) {
            identifier->print(os, indent_level + 1);
        }

        if (value) {
            value->print(os, indent_level + 1);
        }
    }

    MutDeclare::MutDeclare(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value) {
        this->identifier = std::move(identifier);
        this->value = std::move(value);
    }

    void MutDeclare::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "MutDeclare:\n";

        if (identifier) {
            identifier->print(os, indent_level + 1);
        }

        if (value) {
            value->print(os, indent_level + 1);
        }
    }

    AssignVar::AssignVar(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value) {
        this->identifier = std::move(identifier);
        this->value = std::move(value);
    }

    void AssignVar::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "AssignVar:\n";

        if (identifier) {
            identifier->print(os, indent_level + 1);
        }

        if (value) {
            value->print(os, indent_level + 1);
        }
    }

}
