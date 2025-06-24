#pragma once

#include <string>

#include "frontend/ast.h"


namespace frontend {
    frontend::AST parse(const std::string &input_file);
}
