//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef AST_NODES_HPP
#define AST_NODES_HPP
#include <memory>
#include <string>
#include <ostream>

#include "operators.hpp"

namespace parser {

    struct ASTNode {
        virtual ~ASTNode() = default;
        virtual void print(std::ostream& os, int indent) const = 0;
    };

    enum ASTValueType {
        STRING_LITERAL,
        FLOAT,
        INTEGER,
        IDENTIFIER
    };

    std::string ast_val_type_str(ASTValueType val);

    ASTValueType get_var_type_from_node(ASTNode* node);

    struct BuiltInFunc : ASTNode {
        std::string func_name;
        std::unique_ptr<ASTNode> arg;

        BuiltInFunc(const std::string& func_name, std::unique_ptr<ASTNode> arg);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct StringLiteral : ASTNode {
        std::string content;

        StringLiteral(const std::string& content);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct Integer : ASTNode {
        int value;

        Integer(int value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct Float : ASTNode {
        float value;

        Float(float value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct Identifier : ASTNode {
        std::string name;

        Identifier(const std::string& name);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct ImmDeclare : ASTNode {
        std::unique_ptr<ASTNode> identifier;
        std::unique_ptr<ASTNode> value;

        ImmDeclare(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct MutDeclare : ASTNode {
        std::unique_ptr<ASTNode> identifier;
        std::unique_ptr<ASTNode> value;

        MutDeclare(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct AssignVar : ASTNode {
        std::unique_ptr<ASTNode> identifier;
        std::unique_ptr<ASTNode> value;

        AssignVar(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct BinaryOp : ASTNode {
        std::unique_ptr<ASTNode> left;
        std::unique_ptr<ASTNode> right;
        BinaryOperator op;
        ASTValueType left_type;
        ASTValueType right_type;

        BinaryOp(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right, BinaryOperator op, ASTValueType left_type, ASTValueType right_type);
        void print(std::ostream& os, int indent_level) const override;
    };

}

#endif //AST_NODES_HPP
