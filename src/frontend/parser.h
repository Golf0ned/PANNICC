#pragma once

#include <string>

#include "frontend/ast/ast.h"

namespace frontend {
    ast::Program parse(const std::string &input_file);
}
