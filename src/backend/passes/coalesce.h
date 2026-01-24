#pragma once

#include "backend/passes/interference.h"

namespace backend {
    bool tryCoalesce(lir::Program &lir, Interference &interference);
} // namespace backend
