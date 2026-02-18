#include "backend/passes/legalize.h"

namespace backend {
    void legalize(lir::Program &lir) {
        // TODO: push/pop callee-saved registers that are used
        // TODO: allocate/deallocate stack space
        // TODO: replace stack args
        // TODO: save/restore caller-saved registers around function calls
    }
} // namespace backend
