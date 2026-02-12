#include "backend/codegen.h"

namespace backend {
    std::string generateCode(lir::Program &lir) {
        // TODO: generate preamble
        // TODO: validate lir is assembly form
        // TODO: output asm
        return lir.toString();
    }
} // namespace backend
