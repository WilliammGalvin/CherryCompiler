#ifndef AST_NODES_HPP
#define AST_NODES_HPP

#include <memory>
#include <string>
#include <ostream>

#include "operators.hpp"

namespace parser {

    enum ASTValueType {
        STRING_LITERAL,
        FLOAT,
        INTEGER,
        IDENTIFIER,
        BUILTIN_FUNC,
        IMM_DECLARE,
        MUT_DECLARE,
        ASSIGN_VAR,
        BINARY_OP,
    };

    struct ASTNode {
        ASTValueType type;
        virtual ~ASTNode() = default;
        virtual void print(std::ostream& os, int indent) const = 0;
    };

    std::string ast_val_type_str(ASTValueType val);

    ASTValueType get_var_type_from_node(ASTNode* node);

    struct BuiltInFunc final : ASTNode {
        std::string func_name;
        std::unique_ptr<ASTNode> arg;

        BuiltInFunc(const std::string& func_name, std::unique_ptr<ASTNode> arg);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct StringLiteral final : ASTNode {
        std::string content;

        explicit StringLiteral(const std::string& content);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct Integer final : ASTNode {
        int value;

        explicit Integer(int value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct Float final : ASTNode {
        float value;

        explicit Float(float value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct Identifier final : ASTNode {
        std::string name;

        explicit Identifier(const std::string& name);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct ImmDeclare final : ASTNode {
        std::unique_ptr<ASTNode> identifier;
        std::unique_ptr<ASTNode> value;

        ImmDeclare(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct MutDeclare final : ASTNode {
        std::unique_ptr<ASTNode> identifier;
        std::unique_ptr<ASTNode> value;

        MutDeclare(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct AssignVar final : ASTNode {
        std::unique_ptr<ASTNode> identifier;
        std::unique_ptr<ASTNode> value;

        AssignVar(std::unique_ptr<ASTNode> identifier, std::unique_ptr<ASTNode> value);
        void print(std::ostream& os, int indent_level) const override;
    };

    struct BinaryOp final : ASTNode {
        std::unique_ptr<ASTNode> left;
        std::unique_ptr<ASTNode> right;
        BinaryOperator op;

        BinaryOp(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right, BinaryOperator op);

        void print(std::ostream& os, int indent_level) const override;
    };

}

#endif //AST_NODES_HPP
