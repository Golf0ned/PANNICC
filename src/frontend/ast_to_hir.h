#pragma once

#include "frontend/ast/ast.h"
#include "frontend/hir/hir.h"


namespace frontend {
    hir::Program astToHir(const ast::Program& ast);
}
