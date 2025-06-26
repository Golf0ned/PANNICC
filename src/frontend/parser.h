#pragma once

#include <string>

#include "frontend/ast/ast.h"


namespace frontend {
    frontend::AST parse(const std::string &input_file);
}
