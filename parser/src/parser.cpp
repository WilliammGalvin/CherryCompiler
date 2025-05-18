//
// Created by Willam Galvin on 2025-05-17.
//

#include "../include/parser.hpp"

#include <regex>

#include "../include/parse_error.hpp"

namespace parser {

    std::unique_ptr<ASTNode> build_number(const std::vector<lexer::Token>& tokens, size_t* index) {
        std::smatch match;
        const std::regex reg(R"([0-9]+.[0-9]+)");
        const std::string& value_str = tokens[*index].value;

        if (std::regex_match(tokens[*index].value, match, reg)) {
            float val;

            try {
                val = std::stof(value_str);
            } catch (const std::invalid_argument& _) {
                throw ParseError("Invalid float value: '" + value_str + "'.");
            } catch (const std::out_of_range& _) {
                throw ParseError("Value is out of range for float.");
            }

            return std::make_unique<Float>(val);
        }

        int val;

        try {
            val = std::stoi(value_str);
        } catch (const std::invalid_argument& _) {
            throw ParseError("Invalid integer value: '" + value_str + "'.");
        } catch (const std::out_of_range& _) {
            throw ParseError("Value is out of range for integer.");
        }

        return std::make_unique<Integer>(val);
    }

    ASTNode* build_expr(const std::vector<lexer::Token>& tokens, size_t* index) {
        if (tokens[*index].type != lexer::BUILTIN_FUNC) {
            throw ParseError("Expected BUILTIN_FUNC at start of expr.");
        }

        const std::string* func_name = &tokens[*index].value;
        *index += 1;

        switch (tokens[*index].type) {
            case lexer::STRING_LITERAL: {
                if (auto inner = std::make_unique<StringLiteral>(tokens[*index].value)) {
                    *index += 1;
                    return new BuiltInFunc(*func_name, std::move(inner));
                }
            }

            case lexer::NUMBER: {
                std::unique_ptr<ASTNode> inner = build_number(tokens, index);
                *index += 1;
                return new BuiltInFunc(*func_name, std::move(inner));
            }

            default:
                throw ParseError("Expected parameter for function.");
        }
    }

    std::vector<ASTNode*> build_asts_from_tokens(const std::vector<lexer::Token>& tokens) {
        std::vector<std::vector<lexer::Token>> token_sets{};
        std::vector<lexer::Token> current_set{};

        for (const auto& token : tokens) {
            if (token.type == lexer::LINE_END) {
                if (current_set.empty()) {
                    continue;
                }

                token_sets.emplace_back(current_set);
                current_set.clear();
                continue;
            }

            current_set.emplace_back(token);
        }

        std::vector<ASTNode*> asts{};
        for (const auto& token_set : token_sets) {
            size_t index = 0;
            ASTNode* ast = build_expr(token_set, &index);

            if (index < token_set.size()) {
                throw ParseError("Remaining tokens found after parsing.");
            }

            asts.emplace_back(ast);
        }

        return asts;
    }

}
