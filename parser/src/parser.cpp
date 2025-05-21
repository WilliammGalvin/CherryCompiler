#include <regex>

#include "../include/parser.hpp"
#include "../include/defined_functions.hpp"
#include "../include/parse_error.hpp"

namespace parser {

    const lexer::Token& Parser::peek() const {
        return tokens[index];
    }

    const lexer::Token& Parser::backPeek() const {
        return tokens[index - 1];
    }

    bool Parser::is_at_end() const {
        return index >= tokens.size();
    }

    const lexer::Token& Parser::consume() {
        if (!is_at_end()) {
            index++;
        }

        return backPeek();
    }

    void Parser::advance() {
        index++;
    }

    bool Parser::check(const lexer::TokenType type) const {
        return peek().type == type;
    }

    const lexer::Token& Parser::expect(const lexer::TokenType type, const std::string& err_msg) {
        if (peek().type != type) {
            throw ParseError(err_msg);
        }

        return consume();
    }

    void Parser::expect_symbol(const lexer::TokenType type, const std::string& err_msg) {
        if (consume().type != type) {
            throw ParseError(err_msg);
        }
    }

    std::unique_ptr<ASTNode> Parser::build_factor() {
        if (peek().type == lexer::LEFT_PAREN) {
            advance();
            auto expr = build_expr();

            expect_symbol(lexer::RIGHT_PAREN, "Expected closing parenthesis in factor.");

            advance();
            return expr;
        }

        switch (peek().type) {
            case lexer::STRING_LITERAL: {
                const std::string& content = consume().value;
                return std::make_unique<StringLiteral>(content);
            }

            case lexer::FLOAT: {
                float val;

                try {
                    val = std::stof(consume().value);
                } catch (const std::invalid_argument& _) {
                    throw ParseError("Invalid float value: '" + backPeek().value + "'.");
                } catch (const std::out_of_range& _) {
                    throw ParseError("Value is out of range for float.");
                }

                return std::make_unique<Float>(val);
            }

            case lexer::INTEGER: {
                int val;

                try {
                    val = std::stoi(consume().value);
                } catch (const std::invalid_argument& _) {
                    throw ParseError("Invalid integer value: '" + backPeek().value + "'.");
                } catch (const std::out_of_range& _) {
                    throw ParseError("Value is out of range for integer.");
                }

                return std::make_unique<Integer>(val);
            }

            case lexer::IDENTIFIER: {
                const std::string& content = consume().value;
                return std::make_unique<Identifier>(content);
            }

            default:
                throw ParseError("Unexpected token in build factor.");
        }
    }

    std::unique_ptr<ASTNode> Parser::build_term() {
        auto left = build_factor();

        while (!is_at_end()) {
            BinaryOperator op;

            if (peek().type == lexer::MULTIPLY) {
                op = BinaryOperator::MULTIPLY;
            } else if (peek().type == lexer::DIVIDE) {
                op = BinaryOperator::DIVIDE;
            } else {
                break;
            }

            advance();
            auto right = build_factor();

            left = std::make_unique<BinaryOp>(
                std::move(left), std::move(right), op
            );
        }

        return left;
    }

    std::unique_ptr<ASTNode> Parser::build_expr() {
        auto left = build_term();

        while (!is_at_end()) {
            BinaryOperator op;

            if (peek().type == lexer::ADD) {
                op = BinaryOperator::ADD;
            } else if (peek().type == lexer::SUBTRACT) {
                op = BinaryOperator::SUBTRACT;
            } else {
                break;
            }

            advance();
            auto right = build_term();

            left = std::make_unique<BinaryOp>(
                std::move(left), std::move(right), op
            );
        }

        return left;
    }

    std::unique_ptr<ASTNode> Parser::build_imm_declare() {
        auto identifier_token = expect(lexer::IDENTIFIER,
            "Expected valid identifier in immutable declaration.");

        auto identifier = std::make_unique<Identifier>(identifier_token.value);
        expect_symbol(lexer::EQUALS, "Unexpected token in build immutable declaration.");

        auto expr = build_expr();
        return std::make_unique<ImmDeclare>(std::move(identifier), std::move(expr));
    }

    std::unique_ptr<ASTNode> Parser::build_mut_declare() {
        auto identifier_token = expect(lexer::IDENTIFIER,
     "Expected valid identifier in mutable declaration.");

        auto identifier = std::make_unique<Identifier>(identifier_token.value);
        expect_symbol(lexer::EQUALS, "Unexpected token in build mutable declaration.");

        auto expr = build_expr();
        return std::make_unique<MutDeclare>(std::move(identifier), std::move(expr));
    }

    std::unique_ptr<ASTNode> Parser::build_assign_var() {
        auto identifier_token = expect(lexer::IDENTIFIER,
            "Expected valid identifier in value assignment statement.");

        auto identifier = std::make_unique<Identifier>(identifier_token.value);
        expect_symbol(lexer::EQUALS, "Unexpected token in build assignment statement.");

        auto expr = build_expr();
        return std::make_unique<AssignVar>(std::move(identifier), std::move(expr));
    }

    std::unique_ptr<ASTNode> Parser::build_builtin_func_call() {
        if (!defined_functions.contains(peek().value)) {
            throw ParseError("Builtin function '" + peek().value + "' unidentified.");
        }

        auto func_name = consume().value;
        auto expr = build_expr();
        return std::make_unique<BuiltInFunc>(std::move(func_name), std::move(expr));
    }

    std::unique_ptr<ASTNode> Parser::build_statement() {
        std::unique_ptr<ASTNode> stmt;

        if (peek().type == lexer::KEYWORD) {
            if (peek().value == "dec") {
                advance();
                stmt = build_imm_declare();
            } else if (peek().value == "decm") {
                advance();
                stmt = build_mut_declare();
            } else {
                throw ParseError("Unidentified keyword found.");
            }
        } else if (peek().type == IDENTIFIER) {
            stmt = build_assign_var();
        } else if (peek().type == BUILTIN_FUNC) {
            stmt = build_builtin_func_call();
        } else {
            throw ParseError("Unexpected token in build statement.");
        }

        expect_symbol(lexer::LINE_END, "Expected line end after statement.");
        return stmt;
    }

    Parser::Parser(const std::vector<lexer::Token>& tokens)
        : tokens(tokens), index(0) {}

    std::vector<std::unique_ptr<ASTNode>> Parser::build_program() {
        std::vector<std::unique_ptr<ASTNode>> program{};

        while (!is_at_end()) {
            program.emplace_back(build_statement());
        }

        return program;
    }

}
