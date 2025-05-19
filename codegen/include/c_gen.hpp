//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef C_GEN_HPP
#define C_GEN_HPP
#include "../../parser/include/ast_nodes.hpp"
#include <unordered_map>
#include <unordered_set>
#include "c_libs.hpp"
#include "variable.hpp"

namespace codegen {

    class CGen {
        std::unordered_set<CLibrary> libraries{};
        VariableMap variables{};

        parser::ASTNode* fold_binary_op(parser::BinaryOp* node);

        void gen_string_literal(parser::StringLiteral* node, std::ostream& out);
        void gen_float(parser::Float* node, std::ostream& out);
        void gen_integer(parser::Integer* node, std::ostream& out);
        void gen_identifier(parser::Identifier* node, std::ostream& out);

        void gen_primary_value(parser::ASTNode* node, std::ostream& out);
        void gen_value(parser::ASTNode* node, std::ostream& out);

        void gen_builtin_func(parser::BuiltInFunc* node, std::ostream& out);
        void gen_imm_declare(parser::ImmDeclare* node, std::ostream& out);
        void gen_mut_declare(parser::MutDeclare* node, std::ostream& out);
        void gen_assign_var(parser::AssignVar* node, std::ostream& out);

        void gen_statement(parser::ASTNode* ast, std::ostream& out);

        void require_lib(CLibrary lib);

    public:
        void generate(std::vector<std::unique_ptr<parser::ASTNode>>& asts, std::ostream& out);
    };

}

#endif //C_GEN_HPP
