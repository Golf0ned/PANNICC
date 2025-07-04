#pragma once

#include <string>

#include "frontend/ast/ast.h"


namespace frontend {
    frontend::ast::Program parse(const std::string &input_file);
}
