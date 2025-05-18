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
    ASTNode* build_expr(const std::vector<lexer::Token>& tokens, size_t* index);

    std::vector<ASTNode*> build_asts_from_tokens(const std::vector<lexer::Token>& tokens);

}

#endif //PARSER_HPP
