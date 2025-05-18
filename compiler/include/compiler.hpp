//
// Created by Willam Galvin on 2025-05-18.
//

#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <string>

#if defined(_WIN32)
const std::string file_extension = ".exe";
#else
const std::string file_extension = ".out";
#endif

namespace compiler {

    class Compiler {
        std::string input_file;
        std::string output_file;
        std::string compiler_type;

        bool cmd_exists(const std::string& cmd);

        bool compile_c_file(const std::string& src_file, const std::string& compiler);

        void run_binary();

        void clear_screen();

    public:
        Compiler(const std::string& input_path);

        void remove_temp_c_file();
        void compile();
    };

}

#endif //COMPILER_HPP
