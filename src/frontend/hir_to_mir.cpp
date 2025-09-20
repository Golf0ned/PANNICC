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
            mir::Type function_type = toMir(f.getType());

            HIRToMIRVisitor visitor;
            for (auto &i : f.getBody()) {
                i->accept(&visitor);
            }
            std::vector<mir::BasicBlock> basic_blocks = visitor.getResult();

            auto &nameAtom = f.getName();
            std::string name = nameAtom->toString(hir.getSymbolTable());
            functions.emplace_back(function_type, name,
                                   std::move(basic_blocks));
            lowered_functions[nameAtom->getValue()] = &functions.back();
        }
        return mir::Program(std::move(functions));
    }

    std::vector<mir::BasicBlock> HIRToMIRVisitor::getResult() {
        return std::move(basic_blocks);
    }

    mir::Value *HIRToMIRVisitor::resolveAtom(Atom *a) {
        if (a->isIdentifier()) {
            auto load = std::make_unique<mir::InstructionLoad>(
                mir::Type::I64, value_mappings.at(a->getValue()));
            mir::InstructionLoad *load_ptr = load.get();
            cur_instructions.push_back(std::move(load));
            return load_ptr;
        } else {
            auto literal =
                std::make_unique<mir::Literal>(mir::Type::I64, a->getValue());
            mir::Literal *literal_ptr = literal.get();
            cur_literals.push_back(std::move(literal));
            return literal_ptr;
        }
    }

    void HIRToMIRVisitor::visit(hir::Instruction *i) {}

    void HIRToMIRVisitor::visit(hir::Label *l) {
        // TODO
    }

    void HIRToMIRVisitor::visit(hir::InstructionDeclaration *i) {
        mir::Type t = toMir(i->getType());
        auto alloca = std::make_unique<mir::InstructionAlloca>(t);
        value_mappings[i->getVariable()->getValue()] = alloca.get();

        cur_instructions.push_back(std::move(alloca));
    }

    void HIRToMIRVisitor::visit(hir::InstructionAssignValue *i) {
        auto value = resolveAtom(i->getValue().get());
        mir::InstructionAlloca *ptr =
            value_mappings.at(i->getVariable()->getValue());
        auto store = std::make_unique<mir::InstructionStore>(value, ptr);

        cur_instructions.push_back(std::move(store));
    }

    void HIRToMIRVisitor::visit(hir::InstructionAssignBinaryOp *i) {
        mir::Value *left = resolveAtom(i->getLeft().get());
        mir::Value *right = resolveAtom(i->getRight().get());
        mir::BinaryOp op = toMir(i->getOp());
        auto binOp = std::make_unique<mir::InstructionBinaryOp>(
            mir::Type::I64, op, left, right);

        mir::InstructionAlloca *ptr =
            value_mappings.at(i->getVariable()->getValue());
        auto store = std::make_unique<mir::InstructionStore>(binOp.get(), ptr);

        cur_instructions.push_back(std::move(binOp));
        cur_instructions.push_back(std::move(store));
    }

    void HIRToMIRVisitor::visit(hir::InstructionReturn *i) {
        mir::Value *retVal = resolveAtom(i->getValue().get());
        std::unique_ptr<mir::TerminatorReturn> ret =
            std::make_unique<mir::TerminatorReturn>(retVal);

        basic_blocks.emplace_back(std::move(cur_instructions), std::move(ret),
                                  std::move(cur_literals));
        cur_instructions.clear();
        cur_literals.clear();
    }

    void HIRToMIRVisitor::visit(hir::InstructionCall *i) {
        mir::Function *callee =
            lowered_functions.at(i->getCallee()->getValue());
        auto call =
            std::make_unique<mir::InstructionCall>(mir::Type::I64, callee);

        cur_instructions.push_back(std::move(call));
    }

    void HIRToMIRVisitor::visit(hir::InstructionCallAssign *i) {
        mir::Function *callee =
            lowered_functions.at(i->getCallee()->getValue());
        auto call =
            std::make_unique<mir::InstructionCall>(mir::Type::I64, callee);

        mir::InstructionAlloca *ptr =
            value_mappings.at(i->getVariable()->getValue());
        auto store = std::make_unique<mir::InstructionStore>(call.get(), ptr);

        cur_instructions.push_back(std::move(call));
        cur_instructions.push_back(std::move(store));
    }

    void HIRToMIRVisitor::visit(hir::InstructionBranch *i) {
        // TODO
    }

    void HIRToMIRVisitor::visit(hir::InstructionBranchCond *i) {
        // TODO
    }
} // namespace frontend
