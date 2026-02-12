#pragma once

#include "backend/lir/lir.h"

namespace backend {
    std::string generateCode(lir::Program &lir);
}
