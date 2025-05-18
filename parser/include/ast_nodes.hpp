//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef AST_NODES_HPP
#define AST_NODES_HPP
#include <memory>
#include <string>
#include <ostream>

namespace parser {

    struct ASTNode {
        virtual ~ASTNode() = default;
        virtual void print(std::ostream& os, int indent) const = 0;
    };

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

}

#endif //AST_NODES_HPP
