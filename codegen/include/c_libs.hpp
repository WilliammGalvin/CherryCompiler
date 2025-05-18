//
// Created by Willam Galvin on 2025-05-17.
//

#ifndef C_LIBS_HPP
#define C_LIBS_HPP

#include <vector>
#include <string>

namespace codegen {

    enum CLibrary {
        STDIO
    };

    std::string get_library_str(CLibrary lib);

}

#endif //C_LIBS_HPP
