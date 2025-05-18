//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>

#include "ast_nodes.hpp"
#include "../../lexer/include/token.hpp"

namespace parser {

    std::unique_ptr<ASTNode> build_number(const std::vector<lexer::Token>& tokens, size_t* index);
    std::unique_ptr<ASTNode> build_string_literal(const std::vector<lexer::Token>& tokens, size_t* index);
    std::unique_ptr<ASTNode> build_identifier(const std::vector<lexer::Token>& tokens, size_t* index);

    std::unique_ptr<ASTNode> build_value(const std::vector<lexer::Token>& tokens, size_t* index);

    std::unique_ptr<ASTNode> build_imm_declare(const std::vector<lexer::Token>& tokens, size_t* index);
    std::unique_ptr<ASTNode> build_mut_declare(const std::vector<lexer::Token>& tokens, size_t* index);
    std::unique_ptr<ASTNode> build_assign_var(const std::vector<lexer::Token>& tokens, size_t* index);
    std::unique_ptr<ASTNode> build_builtin_func_call(const std::vector<lexer::Token>& tokens, size_t* index);

    std::unique_ptr<ASTNode> build_statement(const std::vector<lexer::Token>& tokens, size_t* index);

    std::vector<std::unique_ptr<ASTNode>> build_program(const std::vector<lexer::Token>& tokens);

}

#endif //PARSER_HPP
