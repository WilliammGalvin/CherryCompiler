#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "variable.hpp"
#include "../../parser/include/ast_nodes.hpp"

namespace codegen {

    ValueVariant extract_value(parser::ASTNode* node, const VariableMap& variables);

    ValueVariant evaluate_binary_op(parser::BinaryOp* bin_op, const VariableMap& variables);

}

#endif //EVALUATOR_HPP
