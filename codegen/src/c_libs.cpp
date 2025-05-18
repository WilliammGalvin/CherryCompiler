//
// Created by Willam Galvin on 2025-05-17.
//

#include <cassert>

#include "../include/c_gen.hpp"

namespace codegen {

    static const std::vector<std::pair<CLibrary, std::string>> c_libraries = {
        { STDIO, "stdio.h" }
    };

    std::string get_library_str(CLibrary lib) {
        for (const auto& [key, value] : c_libraries) {
            if (key == lib) {
                return value;
            }
        }

        assert(false && "C library not found from enum");
        return "";
    }

}