cmake_minimum_required(VERSION 3.28)
project(CherryCompiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(CherryCompiler main.cpp
        lexer/include/token.hpp
        lexer/include/lexer.hpp
        lexer/src/lexer.cpp
        lexer/src/token.cpp
        lexer/include/lex_error.hpp
        lexer/src/lex_error.cpp
        parser/include/parser.hpp
        parser/src/parser.cpp
        parser/include/ast_nodes.hpp
        parser/src/ast_nodes.cpp
        parser/include/parse_error.hpp
        parser/src/parse_error.cpp
        codegen/include/c_gen.hpp
        codegen/src/c_gen.cpp
        codegen/include/c_libs.hpp
        codegen/src/c_libs.cpp
        codegen/include/code_gen_error.hpp
        codegen/src/code_gen_error.cpp
        codegen/include/variable.hpp
        compiler/include/compiler.hpp
        compiler/src/compiler.cpp
        compiler/include/compiler_error.hpp
        compiler/src/compiler_error.cpp
        parser/include/operators.hpp
        parser/src/operators.cpp
        codegen/include/evaluator.hpp
        codegen/src/evaluator.cpp
        parser/include/defined_functions.hpp
        parser/src/defined_functions.cpp
)
