//
// Created by Willam Galvin on 2025-05-17.
//

#include "../include/c_gen.hpp"

#include <cassert>
#include <iostream>

#include "../include/code_gen_error.hpp"
#include <sstream>
#include <variant>

#include "../include/evaluator.hpp"

namespace codegen {

    parser::ASTNode* CGen::fold_binary_op(parser::BinaryOp* node) {
        const auto val = evaluate_binary_op(node, variables);

        if (std::holds_alternative<std::string>(val)) {
            return new parser::StringLiteral(std::get<std::string>(val));
        }

        if (std::holds_alternative<float>(val)) {
            return new parser::Float(std::get<float>(val));
        }

        if (std::holds_alternative<int>(val)) {
            return new parser::Integer(std::get<int>(val));
        }

        throw CodeGenError("Unsupported result in fold_binary_op.");
    }

    void CGen::gen_string_literal(parser::StringLiteral* node, std::ostream& out) {
        out << "\"" << node->content << "\"";
    }

    void CGen::gen_float(parser::Float* node, std::ostream& out) {
        out << std::to_string(node->value) + "f";
    }

    void CGen::gen_integer(parser::Integer* node, std::ostream& out) {
        out << node->value;
    }

    void CGen::gen_identifier(parser::Identifier* node, std::ostream& out) {
        out << node->name;
    }

    void CGen::gen_primary_value(parser::ASTNode* node, std::ostream& out) {
        if (auto str = dynamic_cast<parser::StringLiteral*>(node)) {
            gen_string_literal(str, out);
        } else if (auto f_val = dynamic_cast<parser::Float*>(node)) {
            gen_float(f_val, out);
        } else if (auto i_val = dynamic_cast<parser::Integer*>(node)) {
            gen_integer(i_val, out);
        } else {
            throw CodeGenError("Invalid AST for value.");
        }
    }

    void CGen::gen_value(parser::ASTNode* node, std::ostream& out) {
        if (auto bin_op = dynamic_cast<parser::BinaryOp*>(node)) {
            std::unique_ptr<parser::ASTNode> folded;

            try {
                folded.reset(fold_binary_op(bin_op));
            } catch (...) {
                //
            }

            if (folded) {
                gen_primary_value(folded.get(), out);
                return;
            }
        }

        gen_primary_value(node, out);
    }

    void CGen::gen_builtin_func(parser::BuiltInFunc* node, std::ostream& out) {
        const std::string& name = node->func_name;

        if (name == "print") {
            require_lib(STDIO);

            out << "printf(";

            if (auto iden_val = dynamic_cast<parser::Identifier*>(node->arg.get())) {
                if (!variables.contains(iden_val->name)) {
                    throw CodeGenError("Attempted to use undefined variable '" + iden_val->name + "'.");
                }

                parser::ASTValueType v_type = variables.at(iden_val->name).type;
                assert(v_type != parser::IDENTIFIER);

                switch (variables.at(iden_val->name).type) {
                    case parser::STRING_LITERAL: {
                        out << "\"%s\", " << iden_val->name;
                    } break;
                    case parser::FLOAT: {
                        out << "\"%f\", " << iden_val->name;
                    } break;
                    case parser::INTEGER: {
                        out << "\"%i\", " << iden_val->name;
                    } break;
                    default: break;
                }
            } else if (auto str = dynamic_cast<parser::StringLiteral*>(node->arg.get())) {
                out << "\"%s\", ";
                gen_string_literal(str, out);
            } else if (auto f_val = dynamic_cast<parser::Float*>(node->arg.get())) {
                out << "\"%f\", ";
                gen_float(f_val, out);
            } else if (auto i_val = dynamic_cast<parser::Integer*>(node->arg.get())) {
                out << "\"%i\", ";
                gen_integer(i_val, out);
            }

            out << ");";
            return;
        }

        if (name == "println") {
            require_lib(STDIO);

            out << "printf(";

            if (auto iden_val = dynamic_cast<parser::Identifier*>(node->arg.get())) {
                if (!variables.contains(iden_val->name)) {
                    throw CodeGenError("Attempted to use undefined variable '" + iden_val->name + "'.");
                }

                parser::ASTValueType v_type = variables.at(iden_val->name).type;
                assert(v_type != parser::IDENTIFIER);

                switch (variables.at(iden_val->name).type) {
                    case parser::STRING_LITERAL: {
                        out << R"("%s\n", )" << iden_val->name;
                    } break;
                    case parser::FLOAT: {
                        out << R"("%f\n", )" << iden_val->name;
                    } break;
                    case parser::INTEGER: {
                        out << R"("%i\n", )" << iden_val->name;
                    } break;
                    default: break;
                }
            } else if (auto str = dynamic_cast<parser::StringLiteral*>(node->arg.get())) {
                out << R"("%s\n", )";
                gen_string_literal(str, out);
            } else if (auto f_val = dynamic_cast<parser::Float*>(node->arg.get())) {
                out << R"("%f\n", )";
                gen_float(f_val, out);
            } else if (auto i_val = dynamic_cast<parser::Integer*>(node->arg.get())) {
                out << R"("%i\n", )";
                gen_integer(i_val, out);
            }

            out << ");";
            return;
        }

        throw CodeGenError("Unsupported function '" + node->func_name + "!'.");
    }

    void CGen::gen_imm_declare(parser::ImmDeclare* node, std::ostream &out) {
        auto identifier = dynamic_cast<parser::Identifier*>(node->identifier.get());

        if (!identifier) {
            throw CodeGenError("Expected identifier in immutable declaration.");
        }

        if (variables.contains(identifier->name)) {
            throw CodeGenError("Variable '" + identifier->name + "' already declared.");
        }

        if (auto bin_op = dynamic_cast<parser::BinaryOp*>(node->value.get())) {
            try {
                node->value.reset(fold_binary_op(bin_op));
            } catch (...) {
                //
            }
        }

        auto val_type = parser::get_var_type_from_node(node->value.get());

        std::string c_type;
        switch (val_type) {
            case parser::STRING_LITERAL: c_type = "const char*"; break;
            case parser::FLOAT: c_type = "const float"; break;
            case parser::INTEGER: c_type = "const int"; break;
            default:
                throw CodeGenError("Unsupported type in immutable declaration.");
        }

        ValueVariant init_val;
        bool saw_literal = false;

        if (auto str = dynamic_cast<parser::StringLiteral*>(node->value.get())) {
            init_val = str->content;
            saw_literal = true;
        } else if (auto f_val = dynamic_cast<parser::Float*>(node->value.get())) {
            init_val = f_val->value;
            saw_literal = true;
        } else if (auto i_val = dynamic_cast<parser::Integer*>(node->value.get())) {
            init_val = i_val->value;
            saw_literal = true;
        }

        variables.emplace(
            identifier->name,
            Variable{ val_type, ValueVariant{}, false }
        );

        if (saw_literal) {
            variables[identifier->name].value = std::move(init_val);
        }

        out << c_type << " ";
        gen_identifier(identifier, out);
        out << " = ";
        gen_value(node->value.get(), out);
        out << ";";
    }

    void CGen::gen_mut_declare(parser::MutDeclare* node, std::ostream& out) {
        if (auto bin_op = dynamic_cast<parser::BinaryOp*>(node->value.get())) {
            node->value.reset(fold_binary_op(bin_op));
        }

        parser::ASTValueType val_type;
        if (dynamic_cast<parser::StringLiteral*>(node->value.get())) {
            out << "const char* ";
            val_type = parser::STRING_LITERAL;
        } else if (dynamic_cast<parser::Float*>(node->value.get())) {
            out << "float ";
            val_type = parser::FLOAT;
        } else if (dynamic_cast<parser::Integer*>(node->value.get())) {
            out << "int ";
            val_type = parser::INTEGER;
        } else {
            throw CodeGenError("Invalid variable type for declaration.");
        }

        if (auto identifier = dynamic_cast<parser::Identifier*>(node->identifier.get())) {
            if (variables.contains(identifier->name)) {
                throw CodeGenError("Variable with name '" + identifier->name + "' already defined.");
            }

            ValueVariant val;
            if (auto str = dynamic_cast<parser::StringLiteral*>(node->value.get())) {
                val = str->content;
            } else if (auto f_val = dynamic_cast<parser::Float*>(node->value.get())) {
                val = f_val->value;
            } else if (auto i_val = dynamic_cast<parser::Integer*>(node->value.get())) {
                val = i_val->value;
            }

            variables.insert({
                identifier->name,
                Variable{
                    val_type,
                    val,
                    true
                }
            });

            gen_identifier(identifier, out);
        } else {
            throw CodeGenError("Expected identifier in immutable declaration.");
        }

        out << " = ";
        gen_value(node->value.get(), out);
        out << ";";
    }

    void CGen::gen_assign_var(parser::AssignVar* node, std::ostream& out) {
        if (auto bin_op = dynamic_cast<parser::BinaryOp*>(node->value.get())) {
            node->value.reset(fold_binary_op(bin_op));
        }

        std::string identifier_str;

        if (auto identifier = dynamic_cast<parser::Identifier*>(node->identifier.get())) {
            if (!variables.contains(identifier->name)) {
                throw CodeGenError("Attempting to assign an undefined variable with name '" + identifier->name + "'.");
            }

            identifier_str = identifier->name;
            gen_identifier(identifier, out);
        } else {
            throw CodeGenError("Expected identifier in immutable declaration.");
        }

        const auto prev_state = variables.at(identifier_str);

        if (!prev_state.muttable) {
            throw CodeGenError("Attempting to mutate an immutable variable.");
        }

        const auto new_type = parser::get_var_type_from_node(
            node->value.get()
        );

        if (prev_state.type != new_type) {
            throw CodeGenError(
                "Attempted to assign wrong type to variable '" + identifier_str + "':\n" +
                parser::ast_val_type_str(prev_state.type) + " -> " + parser::ast_val_type_str(new_type)
            );
        }

        out << " = ";
        gen_value(node->value.get(), out);
        out << ";";
    }

    void CGen::gen_statement(parser::ASTNode* ast, std::ostream& out) {
        if (auto imm_declare = dynamic_cast<parser::ImmDeclare*>(ast)) {
            gen_imm_declare(imm_declare, out);
        } else if (auto mut_declare = dynamic_cast<parser::MutDeclare*>(ast)) {
            gen_mut_declare(mut_declare, out);
        } else if (auto assign_var = dynamic_cast<parser::AssignVar*>(ast)) {
            gen_assign_var(assign_var, out);
        } else if (auto builtin_func = dynamic_cast<parser::BuiltInFunc*>(ast)) {
            gen_builtin_func(builtin_func, out);
        } else {
            throw CodeGenError("Unidentified statement AST.");
        }
    }

    void CGen::generate(std::vector<std::unique_ptr<parser::ASTNode>>& asts, std::ostream& out) {
        std::ostringstream body;
        std::ostringstream includes;

        body << "int main(void) {\n";
        for (size_t i = 0; i < asts.size(); i++) {
            gen_statement(asts[i].get(), body);

            if (i != asts.size() - 1) {
                body << "\n";
            }
        }
        body << "\nreturn 0;\n}\n";

        for (const auto& lib : libraries) {
            includes << "#include <" << get_library_str(lib) << ">\n";
        }

        out << includes.str();
        out << body.str();
    }

    void CGen::require_lib(CLibrary lib) {
        libraries.insert(lib);
    }

}
