#pragma once

#include "backend/lir/lir.h"

namespace backend {
    class CodeGenVisitor : public lir::ToStringVisitor {
    public:
        void generateFunctionLabel(lir::Function *f);
        void generatePreamble(lir::Function *f);
        void generatePostamble(lir::Function *f);

        void visit(lir::InstructionCall *i) override;
    };

    std::string generateCode(lir::Program &lir);
} // namespace backend
