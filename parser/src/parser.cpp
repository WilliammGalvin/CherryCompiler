//
// Created by Willam Galvin on 2025-05-17.
//

#include <cassert>
#include <regex>
#include <unordered_set>

#include "../include/parser.hpp"
#include "../include/parse_error.hpp"

namespace parser {

    std::unique_ptr<ASTNode> build_number(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        assert(
            tokens[*index].type == lexer::NUMBER &&
            "Number expectes token type NUMBER."
        );

        std::smatch match;
        const std::regex reg(R"([0-9]+\.[0-9]+)");
        const std::string& value_str = tokens[*index].value;

        // Match against tokens with Float values
        if (std::regex_match(tokens[*index].value, match, reg)) {
            float val;

            try {
                val = std::stof(value_str);
            } catch (const std::invalid_argument& _) {
                throw ParseError("Invalid float value: '" + value_str + "'.");
            } catch (const std::out_of_range& _) {
                throw ParseError("Value is out of range for float.");
            }

            *index += 1;
            return std::make_unique<Float>(val);
        }

        // Otherwise default to Integer
        int val;

        try {
            val = std::stoi(value_str);
        } catch (const std::invalid_argument& _) {
            throw ParseError("Invalid integer value: '" + value_str + "'.");
        } catch (const std::out_of_range& _) {
            throw ParseError("Value is out of range for integer.");
        }

        *index += 1;
        return std::make_unique<Integer>(val);
    }

    std::unique_ptr<ASTNode> build_string_literal(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        assert(
            tokens[*index].type == lexer::STRING_LITERAL &&
            "StringLiteral expectes token type STRING_LITERAL."
        );

        const std::string& val = tokens[*index].value;
        *index += 1;

        return std::make_unique<StringLiteral>(val);
    }

    std::unique_ptr<ASTNode> build_identifier(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        assert(
            tokens[*index].type == lexer::IDENTIFIER &&
            "Identifier expectes token type IDENTIFIER."
        );

        const std::string& name = tokens[*index].value;
        *index += 1;

        return std::make_unique<Identifier>(name);
    }

    std::unique_ptr<ASTNode> build_binary_expr(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        //
    }

    std::unique_ptr<ASTNode> build_primary_value(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        switch (tokens[*index].type) {
            case lexer::STRING_LITERAL:
                return build_string_literal(tokens, index);
            case lexer::NUMBER:
                return build_number(tokens, index);
            case lexer::IDENTIFIER:
                return build_identifier(tokens, index);
            default:
                throw ParseError("Invalid build primary value call.");
        }
    }

    std::unique_ptr<ASTNode> build_value(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        auto left = build_primary_value(tokens, index);

        while (*index < tokens.size()) {
            BinaryOperator op;

            switch (tokens[*index].type) {
                case lexer::ADD: op = ADD; break;
                case lexer::SUBTRACT: op = SUBTRACT; break;
                case lexer::MULTIPLY: op = MULTIPLY; break;
                case lexer::DIVIDE: op = DIVIDE; break;
                default: return left;
            }

            *index += 1;
            auto right = build_primary_value(tokens, index);

            auto get_val_type = [](ASTNode* node) -> ASTValueType {
                if (dynamic_cast<Float*>(node)) return FLOAT;
                if (dynamic_cast<Integer*>(node)) return INTEGER;
                if (dynamic_cast<StringLiteral*>(node)) return STRING_LITERAL;
                if (dynamic_cast<Identifier*>(node)) return IDENTIFIER;
                if (auto bin = dynamic_cast<BinaryOp*>(node)) return bin->left_type;
                throw ParseError("Cannot determine value type of operand.");
            };

            ASTValueType left_type = get_val_type(left.get());
            ASTValueType right_type = get_val_type(right.get());

            left = std::make_unique<BinaryOp>(std::move(left), std::move(right), op, left_type, right_type);
        }

        return left;
    }

    std::unique_ptr<ASTNode> build_imm_declare(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        assert(
            tokens[*index].type == lexer::KEYWORD &&
            tokens[*index].value == "dec" &&
            "ImmDeclare expects token type KEYWORD with value 'dec'."
        );

        *index += 1;

        if (tokens[*index].type != lexer::IDENTIFIER) {
            throw ParseError("Expected identifier in immutable declaration.");
        }

        auto identifier = build_identifier(tokens, index);

        if (tokens[*index].type != lexer::EQUALS) {
            throw ParseError("Expected equal symbol in immutable declaration.");
        }

        *index += 1;
        auto value = build_value(tokens, index);
        return std::make_unique<ImmDeclare>(std::move(identifier), std::move(value));
    }

    std::unique_ptr<ASTNode> build_mut_declare(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        assert(
            tokens[*index].type == lexer::KEYWORD &&
            tokens[*index].value == "decm" &&
            "MutDeclare expects token type KEYWORD with value 'decm'."
        );

        *index += 1;

        if (tokens[*index].type != lexer::IDENTIFIER) {
            throw ParseError("Expected identifier in mutable declaration.");
        }

        auto identifier = build_identifier(tokens, index);

        if (tokens[*index].type != lexer::EQUALS) {
            throw ParseError("Expected equal symbol in mutable declaration.");
        }

        *index += 1;
        auto value = build_value(tokens, index);
        return std::make_unique<MutDeclare>(std::move(identifier), std::move(value));
    }

    std::unique_ptr<ASTNode> build_assign_var(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        auto identifier = build_identifier(tokens, index);

        if (tokens[*index].type != lexer::EQUALS) {
            throw ParseError("Expected equal symbol in assignment.");
        }

        *index += 1;
        auto value = build_value(tokens, index);
        return std::make_unique<AssignVar>(std::move(identifier), std::move(value));
    }

    std::unique_ptr<ASTNode> build_builtin_func_call(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        assert(tokens[*index].type == lexer::BUILTIN_FUNC);
        const std::unordered_set<std::string> builtin_func_names = {
            "print", "println"
        };

        const std::string& func_name = tokens[*index].value;
        if (!builtin_func_names.contains(func_name)) {
            throw ParseError("Builtin function '" + std::string(func_name) + "!' not found.");
        }

        *index += 1;
        auto value = build_value(tokens, index);
        return std::make_unique<BuiltInFunc>(func_name, std::move(value));
    }

    std::unique_ptr<ASTNode> build_statement(
        const std::vector<lexer::Token>& tokens,
        size_t* index
    ) {
        const lexer::Token& token = tokens[*index];

        if (token.type == lexer::KEYWORD) {
            if (token.value == "dec") {
                return build_imm_declare(tokens, index);
            }

            if (token.value == "decm") {
                return build_mut_declare(tokens, index);
            }

            throw ParseError("Keyword '" + token.value + "' is unidentified.");
        }

        if (token.type == lexer::IDENTIFIER) {
            return build_assign_var(tokens, index);
        }

        if (token.type == lexer::BUILTIN_FUNC) {
            return build_builtin_func_call(tokens, index);
        }

        throw ParseError("Unrecognized statement type at beginning of line.");
    }

    std::vector<std::unique_ptr<ASTNode>> build_program(
        const std::vector<lexer::Token>& tokens
    ) {
        std::vector<std::unique_ptr<ASTNode>> asts{};
        size_t index = 0;

        while (index < tokens.size()) {
            if (tokens[index].type == lexer::LINE_END) {
                index++;
                continue;
            }

            auto stmt = build_statement(tokens, &index);

            if (index >= tokens.size() || tokens[index].type != lexer::LINE_END) {
                throw ParseError("Expected end of line after statement");
            }

            index++;
            asts.emplace_back(std::move(stmt));
        }

        return asts;
    }

}
