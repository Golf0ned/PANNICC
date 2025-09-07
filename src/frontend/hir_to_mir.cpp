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
    std::unordered_map<uint64_t, mir::Function *> lowered_functions;

    mir::Program hirToMir(hir::Program &hir) {
        std::vector<mir::Function> functions;
        functions.reserve(hir.getFunctions().size());

        for (auto &f : hir.getFunctions()) {
            mir::Type function_type = toMIR(f.getType());

            HIRToMIRVisitor visitor;
            for (auto i : f.getBody()) {
                i->accept(&visitor);
            }
            std::vector<mir::BasicBlock> basic_blocks = visitor.getResult();

            auto nameAtom = f.getName();
            std::string name = nameAtom->toString(hir.getSymbolTable());
            functions.emplace_back(function_type, name, basic_blocks);
            lowered_functions[nameAtom->getValue()] = &functions.back();
        }
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

    void HIRToMIRVisitor::visit(hir::Instruction *i) {}

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

    void HIRToMIRVisitor::visit(hir::InstructionReturn *i) {
        mir::Value *retVal = resolveAtom(i->getValue());
        mir::TerminatorReturn *ret = new mir::TerminatorReturn(retVal);
        basic_blocks.emplace_back(cur_instructions, ret);
        cur_instructions.clear();
    }

    void HIRToMIRVisitor::visit(hir::InstructionCall *i) {
        mir::Function *callee =
            lowered_functions.at(i->getCallee()->getValue());
        mir::InstructionCall *call =
            new mir::InstructionCall(mir::Type::I64, callee);
        cur_instructions.push_back(call);
    }

    void HIRToMIRVisitor::visit(hir::InstructionCallAssign *i) {
        mir::Function *callee =
            lowered_functions.at(i->getCallee()->getValue());
        mir::InstructionCall *call =
            new mir::InstructionCall(mir::Type::I64, callee);
        cur_instructions.push_back(call);

        mir::Value *ptr = value_mappings.at(i->getVariable()->getValue());
        mir::Value *value = resolveAtom(i->getVariable());
        mir::InstructionStore *store = new mir::InstructionStore(value, ptr);
        cur_instructions.push_back(store);
    }
} // namespace frontend
