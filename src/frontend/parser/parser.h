#pragma once

#include "frontend/ast/ast.h"

namespace frontend {

ast::Program parse_file(const std::string &input_file);
ast::Program parse_string(const std::string &input);

} // namespace frontend
