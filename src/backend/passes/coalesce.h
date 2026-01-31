#pragma once

#include "backend/passes/interference.h"

namespace backend {
    bool tryCoalesce(lir::Function *f, Interference &interference);
} // namespace backend
