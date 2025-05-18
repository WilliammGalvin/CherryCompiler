//
// Created by Willam Galvin on 2025-05-18.
//

#include "../include/compiler.hpp"

#include <iostream>
#include <filesystem>

#include "../include/compiler_error.hpp"

namespace compiler {

    bool Compiler::cmd_exists(const std::string& cmd) {
    #if defined(_WIN32)
        std::string check = "where " + cmd + " > nul 2>&1";
    #else
        std::string check = "command -v " + cmd + " > /dev/null 2>&1";
    #endif
        return std::system(check.c_str()) == 0;
    }

    bool Compiler::compile_c_file(const std::string& src_file, const std::string& compiler) {
        std::string command = compiler + " " + src_file + " -o " + output_file;
        return std::system(command.c_str()) == 0;
    }

    void Compiler::run_binary() {
        std::string command = "./" + output_file;

    #if defined(_WIN32)
        command = output_file;
    #endif
        std::system(command.c_str());
    }

    void Compiler::clear_screen() {
    #if defined(_WIN32)
        std::system("cls");
    #else
        std::system("clear");
    #endif
    }

    Compiler::Compiler(const std::string& input_path) {
        std::filesystem::path p(input_path);
        input_file = p.string();
        output_file = p.parent_path().string() + "/" +
            p.stem().string() + file_extension;
    }

    void Compiler::remove_temp_c_file() {
        std::error_code ec;
        std::filesystem::remove(input_file, ec);

        if (ec) {
            std::cout << "Warning: Unable to delete C source file:\n" << ec.message() << std::endl;
        }
    }

    void Compiler::compile() {
        if (cmd_exists("gcc")) {
            compiler_type = "gcc";
            std::cout << "Compiling using gcc..." << std::endl;
        } else if (cmd_exists("clang")) {
            compiler_type = "clang";
            std::cout << "Compiling using clang..." << std::endl;
        } else {
            throw CompilerError("Compatible C compiler not found. Please use either GCC or Clang.");
        }

        if (!compile_c_file(input_file, compiler_type)) {
            throw CompilerError("Failed to compile C source file.");
        }

        std::cout << "Compiled source file.\nExecuting..." << std::endl;
        clear_screen();

        std::cout << std::endl;
        run_binary();
    }

};