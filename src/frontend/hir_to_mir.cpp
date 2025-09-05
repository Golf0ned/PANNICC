#include "frontend/hir_to_mir.h"
#include "frontend/hir/hir.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/type.h"
#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/operator.h"
#include "middleend/mir/type.h"

namespace mir = middleend::mir;

namespace frontend {
    static uint64_t counter = 0;

    mir::Program hirToMir(hir::Program &hir) {
        std::vector<mir::Function> functions;

        for (auto &f : hir.getFunctions()) {
            mir::Type function_type = toMIR(f.getType());
            uint64_t function_id = counter++;

            HIRToMIRVisitor *visitor;
            for (auto i : f.getBody())
                i->accept(visitor);
            std::vector<mir::BasicBlock> basic_blocks = visitor->getResult();

            functions.emplace_back(function_type, function_id, basic_blocks);
        }
        // TODO: figure out function name mapping
        return mir::Program(functions);
    }

    std::vector<mir::BasicBlock> HIRToMIRVisitor::getResult() {
        return basic_blocks;
    }

    mir::Value *HIRToMIRVisitor::resolveAtom(Atom *a) {
        if (a->isIdentifier()) {
            mir::InstructionLoad *load = new mir::InstructionLoad(
                mir::Type::I64, value_mappings.at(a->getValue()));
            cur_instructions.push_back(load);
            return load;
        } else {
            mir::Literal *value =
                new mir::Literal(mir::Type::I64, a->getValue());
            return value;
        }
    }

    void HIRToMIRVisitor::visit(hir::InstructionDeclaration *i) {
        mir::Type t = toMIR(i->getType());
        mir::InstructionAlloca *alloca = new mir::InstructionAlloca(t);
        cur_instructions.push_back(alloca);
        value_mappings[i->getVariable()->getValue()] = alloca;
    }

    void HIRToMIRVisitor::visit(hir::InstructionAssignValue *i) {
        mir::Value *ptr = value_mappings.at(i->getVariable()->getValue());
        mir::Value *value = resolveAtom(i->getValue());
        mir::InstructionStore *store = new mir::InstructionStore(value, ptr);
        cur_instructions.push_back(store);
    }

    void HIRToMIRVisitor::visit(hir::InstructionAssignBinaryOp *i) {
        mir::Value *left = resolveAtom(i->getLeft());
        mir::Value *right = resolveAtom(i->getRight());
        mir::BinaryOp op = toMIR(i->getOp());
        mir::InstructionBinaryOp *binOp =
            new mir::InstructionBinaryOp(mir::Type::I64, op, left, right);

        mir::Value *ptr = value_mappings.at(i->getVariable()->getValue());
        mir::InstructionStore *store = new mir::InstructionStore(binOp, ptr);
        cur_instructions.push_back(store);
    }

    void HIRToMIRVisitor::visit(hir::InstructionReturn *i) {}

    void HIRToMIRVisitor::visit(hir::InstructionCall *i) {}

    void HIRToMIRVisitor::visit(hir::InstructionCallAssign *i) {}
} // namespace frontend
