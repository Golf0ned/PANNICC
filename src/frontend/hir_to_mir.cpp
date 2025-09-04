#include "frontend/hir_to_mir.h"
#include "frontend/hir/hir.h"
#include "frontend/utils/type.h"
#include "middleend/mir/mir.h"

namespace mir = middleend::mir;

namespace frontend {
    mir::Program hirToMir(hir::Program &hir) {
        std::vector<mir::Function> functions;

        for (auto &f : hir.getFunctions()) {
            mir::Type function_type = toMIRType(f.getType());
            uint64_t function_id = 0; // TODO: get real id

            HIRToMIRVisitor *visitor;
            for (auto i : f.getBody())
                i->accept(visitor);
            std::vector<mir::BasicBlock> basic_blocks = visitor->getResult();

            functions.emplace_back(function_type, function_id, basic_blocks);
        }

        return mir::Program(functions);
    }

    std::vector<mir::BasicBlock> HIRToMIRVisitor::getResult() {
        return basic_blocks;
    }

    // TODO: implement below
    void HIRToMIRVisitor::visit(hir::InstructionDeclaration *i) {}

    void HIRToMIRVisitor::visit(hir::InstructionAssignValue *i) {}

    void HIRToMIRVisitor::visit(hir::InstructionAssignBinaryOp *i) {}

    void HIRToMIRVisitor::visit(hir::InstructionReturn *i) {}

    void HIRToMIRVisitor::visit(hir::InstructionCall *i) {}

    void HIRToMIRVisitor::visit(hir::InstructionCallAssign *i) {}
} // namespace frontend
