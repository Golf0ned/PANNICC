#pragma once

#include "backend/lir/lir.h"
#include "middleend/mir/mir.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir);
}
