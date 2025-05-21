#include "../include/defined_functions.hpp"

namespace parser {

    std::unordered_map<std::string, DefinedFunction> defined_functions = {
        { "print!", PRINT },
        { "println!", PRINTLN },
    };

}