//
// Created by Willam Galvin on 2025-05-18.
//

#include "../include/evaluator.hpp"

#include <variant>

#include "../include/code_gen_error.hpp"

namespace codegen {

    ValueVariant extract_value(parser::ASTNode* node, const VariableMap& variables) {
        if (auto iden_val = dynamic_cast<parser::Identifier*>(node)) {
            if (!variables.contains(iden_val->name)) {
                throw CodeGenError("Attempting to access unidentified variable '" + iden_val->name + "'.");
            }

            return variables.at(iden_val->name).value;
        }

        if (auto str = dynamic_cast<parser::StringLiteral*>(node)) {
            return str->content;
        }

        if (auto f_val = dynamic_cast<parser::Float*>(node)) {
            return f_val->value;
        }

        if (auto i_val = dynamic_cast<parser::Integer*>(node)) {
            return i_val->value;
        }

        throw CodeGenError("Unsupported value type in binary operation.");
    }

    ValueVariant evaluate_binary_op(parser::BinaryOp* bin_op, const VariableMap& variables) {
        ValueVariant left_val;
        ValueVariant right_val;

        if (auto left_bin = dynamic_cast<parser::BinaryOp*>(bin_op->left.get())) {
            left_val = evaluate_binary_op(left_bin, variables);
        } else {
            left_val = extract_value(bin_op->left.get(), variables);
        }

        if (auto right_bin = dynamic_cast<parser::BinaryOp*>(bin_op->right.get())) {
            right_val = evaluate_binary_op(right_bin, variables);
        } else {
            right_val = extract_value(bin_op->right.get(), variables);
        }

        return std::visit(
            [bin_op]<typename L, typename  R>(L&& lhs, R&& rhs) -> ValueVariant {
                using LD = std::decay_t<decltype(lhs)>;
                using RD = std::decay_t<decltype(rhs)>;

                // Same types
                if constexpr (std::is_same_v<LD, std::string> && std::is_same_v<RD, std::string>) {
                    if (bin_op->op == parser::ADD) {
                        return lhs + rhs;
                    }

                    throw CodeGenError("Attempted invalid string binary operation.");
                }

                if constexpr (std::is_same_v<LD, float> && std::is_same_v<RD, float>) {
                    const auto l = static_cast<float>(lhs);
                    const auto r = static_cast<float>(rhs);

                    switch (bin_op->op) {
                        case parser::ADD: return l + r;
                        case parser::SUBTRACT: return l - r;
                        case parser::MULTIPLY: return l * r;
                        case parser::DIVIDE: return l / r;
                        default: throw CodeGenError("Attempted invalid float binary operation.");
                    }
                }

                if constexpr (std::is_same_v<LD, int> && std::is_same_v<RD, int>) {
                    const auto l = static_cast<int>(lhs);
                    const auto r = static_cast<int>(rhs);

                    switch (bin_op->op) {
                        case parser::ADD: return l + r;
                        case parser::SUBTRACT: return l - r;
                        case parser::MULTIPLY: return l * r;
                        case parser::DIVIDE: return l / r;
                        default: throw CodeGenError("Attempted invalid mixed number binary operation.");
                    }
                }

                // String concat
                const bool is_left_str = std::is_same_v<LD, std::string>;
                const bool is_right_str = std::is_same_v<RD, std::string>;

                if constexpr (
                    std::is_same_v<LD, std::string> ||
                    std::is_same_v<RD, std::string>
                ) {
                    std::string l;
                    std::string r;

                    if constexpr (std::is_same_v<LD, std::string>) {
                        l = lhs;
                    } else if constexpr (std::is_same_v<LD, float>) {
                        l = std::to_string(lhs);
                    } else {
                        l = std::to_string(lhs);
                    }

                    if constexpr (std::is_same_v<RD, std::string>) {
                        r = rhs;
                    } else if constexpr (std::is_same_v<RD, float>) {
                        r = std::to_string(rhs);
                    } else {
                        r = std::to_string(rhs);
                    }

                    switch (bin_op->op) {
                        case parser::ADD: return l + r;
                        default: throw CodeGenError("Attempted invalid string binary operation.");
                    }
                }

                // Type promotion
                if constexpr (
                    (std::is_same_v<LD, int> && std::is_same_v<RD, float>) ||
                    (std::is_same_v<LD, float> && std::is_same_v<RD, int>)
                ) {
                    const auto l = static_cast<float>(lhs);
                    const auto r = static_cast<float>(rhs);

                    switch (bin_op->op) {
                        case parser::ADD: return l + r;
                        case parser::SUBTRACT: return l - r;
                        case parser::MULTIPLY: return l * r;
                        case parser::DIVIDE: return l / r;
                        default: throw CodeGenError("Attempted invalid int binary operation.");
                    }
                }

                throw CodeGenError("Attempted binary operation on uncompatible types.");
        }, left_val, right_val);
    }

}
