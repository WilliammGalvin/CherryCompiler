#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>

#include "ast_nodes.hpp"
#include "../../lexer/include/token.hpp"

namespace parser {

    class Parser {
        const std::vector<lexer::Token>& tokens;
        size_t index;

        [[nodiscard]] const lexer::Token& peek() const;
        [[nodiscard]] const lexer::Token& backPeek() const;
        [[nodiscard]] bool is_at_end() const;
        [[nodiscard]] const lexer::Token& consume();
        void advance();
        [[nodiscard]] bool check(lexer::TokenType type) const;
        [[nodiscard]] const lexer::Token& expect(lexer::TokenType type, const std::string& err_msg);
        void expect_symbol(lexer::TokenType type, const std::string& err_msg);

        std::unique_ptr<ASTNode> build_factor();
        std::unique_ptr<ASTNode> build_term();
        std::unique_ptr<ASTNode> build_expr();

        std::unique_ptr<ASTNode> build_imm_declare();
        std::unique_ptr<ASTNode> build_mut_declare();
        std::unique_ptr<ASTNode> build_assign_var();
        std::unique_ptr<ASTNode> build_builtin_func_call();

        std::unique_ptr<ASTNode> build_statement();

    public:
        explicit Parser(const std::vector<lexer::Token>& tokens);

        std::vector<std::unique_ptr<ASTNode>> build_program();
    };

}

#endif //PARSER_HPP
