//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef C_GEN_HPP
#define C_GEN_HPP
#include "../../parser/include/ast_nodes.hpp"
#include <unordered_set>
#include "c_libs.hpp"

namespace codegen {

    class CGen {
        std::unordered_set<CLibrary> libraries{};

        void gen_string_literal(parser::StringLiteral* node, std::ostream& out);
        void gen_float(parser::Float* node, std::ostream& out);
        void gen_integer(parser::Integer* node, std::ostream& out);
        void gen_builtin_func(parser::BuiltInFunc* node, std::ostream& out);
        void gen_program(parser::ASTNode* ast, std::ostream& out);

        void require_lib(CLibrary lib);

    public:
        void generate(std::vector<parser::ASTNode*>& asts, std::ostream& out);
    };

}

#endif //C_GEN_HPP
