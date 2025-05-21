#ifndef BUILTIN_FUNCTIONS_HPP
#define BUILTIN_FUNCTIONS_HPP

#include <unordered_map>
#include <string>

namespace parser {

    enum DefinedFunction {
        PRINT,
        PRINTLN
    };

    extern std::unordered_map<std::string, DefinedFunction> defined_functions;

}

#endif //BUILTIN_FUNCTIONS_HPP
