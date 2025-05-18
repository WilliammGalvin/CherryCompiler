//
// Created by Willam Galvin on 2025-05-17.
//

#include "../include/c_gen.hpp"

#include <cassert>
#include <iostream>

#include "../include/code_gen_error.hpp"
#include <sstream>

namespace codegen {

    void CGen::gen_string_literal(parser::StringLiteral* node, std::ostream& out) {
        out << "\"" << node->content << "\"";
    }

    void CGen::gen_float(parser::Float* node, std::ostream& out) {
        out << std::to_string(node->value) + "f";
    }

    void CGen::gen_integer(parser::Integer* node, std::ostream& out) {
        out << node->value;
    }

    void CGen::gen_builtin_func(parser::BuiltInFunc* node, std::ostream& out) {
        const std::string& name = node->func_name;

        if (name == "print") {
            require_lib(STDIO);

            out << "printf(";

            if (dynamic_cast<parser::StringLiteral*>(node->arg.get())) {
                gen_program(node->arg.get(), out);
            } else if (dynamic_cast<parser::Float*>(node->arg.get())) {
                out << "\"%f\", ";
                gen_program(node->arg.get(), out);
            } else if (dynamic_cast<parser::Integer*>(node->arg.get())) {
                out << "\"%i\", ";
                gen_program(node->arg.get(), out);
            }

            out << ");";
            return;
        }

        if (name == "println") {
            require_lib(STDIO);

            out << "printf(";

            if (auto str_node = dynamic_cast<parser::StringLiteral*>(node->arg.get())) {
                str_node->content.append("\\n");
                gen_program(node->arg.get(), out);
            } else if (dynamic_cast<parser::Float*>(node->arg.get())) {
                out << R"("%f\n", )";
                gen_program(node->arg.get(), out);
            } else if (dynamic_cast<parser::Integer*>(node->arg.get())) {
                out << R"("%i\n", )";
                gen_program(node->arg.get(), out);
            }

            out << ");";
            return;
        }

        throw CodeGenError("Unsupported function '" + node->func_name + "'.");
    }

    void CGen::gen_program(parser::ASTNode* ast, std::ostream& out) {
        if (const auto str = dynamic_cast<parser::StringLiteral*>(ast)) {
            gen_string_literal(str, out);
        } else if (const auto builtin = dynamic_cast<parser::BuiltInFunc*>(ast)) {
            gen_builtin_func(builtin, out);
        } else if (const auto _float = dynamic_cast<parser::Float*>(ast)) {
            gen_float(_float, out);
        } else if (const auto _int = dynamic_cast<parser::Integer*>(ast)) {
            gen_integer(_int, out);
        } else {
            throw CodeGenError("Attempted to code gen an invalid AST node.");
        }
    }

    void CGen::generate(std::vector<parser::ASTNode*>& asts, std::ostream& out) {
        std::ostringstream body;
        std::ostringstream includes;

        body << "int main(void) {\n";
        for (size_t i = 0; i < asts.size(); i++) {
            gen_program(asts[i], body);

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
