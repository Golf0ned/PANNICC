#pragma once

#include <unordered_set>

#include "backend/lir/lir.h"

namespace backend {
using RegisterSet = std::unordered_set<lir::Register *>;

class Liveness {
public:
    Liveness(std::vector<RegisterSet> gen, std::vector<RegisterSet> kill,
             std::vector<RegisterSet> in, std::vector<RegisterSet> out);
    const std::vector<RegisterSet> &getGen() const;
    const std::vector<RegisterSet> &getKill() const;
    const std::vector<RegisterSet> &getIn() const;
    const std::vector<RegisterSet> &getOut() const;

private:
    std::vector<RegisterSet> gen, kill, in, out;
};

Liveness computeLiveness(lir::Function *f, lir::OperandManager *om);
void printLiveness(lir::Function *f, Liveness &l);

class GenSetVisitor : public lir::InstructionVisitor {
public:
    GenSetVisitor(lir::OperandManager *om);

    RegisterSet getResult();
    void checkOperand(lir::Operand *o);
    void addRegister(lir::RegisterNum reg);

    void visit(lir::Instruction *i) override;
    void visit(lir::Label *l) override;
    void visit(lir::InstructionMov *i) override;
    void visit(lir::InstructionPush *i) override;
    void visit(lir::InstructionPop *i) override;
    void visit(lir::InstructionConvert *i) override;
    void visit(lir::InstructionBinaryOp *i) override;
    void visit(lir::InstructionSpecialOp *i) override;
    void visit(lir::InstructionLea *i) override;
    void visit(lir::InstructionCmp *i) override;
    void visit(lir::InstructionJmp *i) override;
    void visit(lir::InstructionCJmp *i) override;
    void visit(lir::InstructionCall *i) override;
    void visit(lir::InstructionRet *i) override;
    void visit(lir::InstructionUnknown *i) override;

private:
    RegisterSet gen;
    lir::OperandManager *om;
};

class KillSetVisitor : public lir::InstructionVisitor {
public:
    KillSetVisitor(lir::OperandManager *om);

    RegisterSet getResult();
    void checkOperand(lir::Operand *o);
    void addRegister(lir::RegisterNum reg);

    void visit(lir::Instruction *i) override;
    void visit(lir::Label *l) override;
    void visit(lir::InstructionMov *i) override;
    void visit(lir::InstructionPush *i) override;
    void visit(lir::InstructionPop *i) override;
    void visit(lir::InstructionConvert *i) override;
    void visit(lir::InstructionBinaryOp *i) override;
    void visit(lir::InstructionSpecialOp *i) override;
    void visit(lir::InstructionLea *i) override;
    void visit(lir::InstructionCmp *i) override;
    void visit(lir::InstructionJmp *i) override;
    void visit(lir::InstructionCJmp *i) override;
    void visit(lir::InstructionCall *i) override;
    void visit(lir::InstructionRet *i) override;
    void visit(lir::InstructionUnknown *i) override;

private:
    RegisterSet kill;
    lir::OperandManager *om;
};

class SuccessorVisitor : public lir::InstructionVisitor {
public:
    SuccessorVisitor(lir::Function *f);

    std::vector<int> getResult();

    void visit(lir::Instruction *i) override;
    void visit(lir::Label *l) override;
    void visit(lir::InstructionMov *i) override;
    void visit(lir::InstructionPush *i) override;
    void visit(lir::InstructionPop *i) override;
    void visit(lir::InstructionConvert *i) override;
    void visit(lir::InstructionBinaryOp *i) override;
    void visit(lir::InstructionSpecialOp *i) override;
    void visit(lir::InstructionLea *i) override;
    void visit(lir::InstructionCmp *i) override;
    void visit(lir::InstructionJmp *i) override;
    void visit(lir::InstructionCJmp *i) override;
    void visit(lir::InstructionCall *i) override;
    void visit(lir::InstructionRet *i) override;
    void visit(lir::InstructionUnknown *i) override;

private:
    std::unordered_map<lir::Instruction *, int> next_index;
    std::unordered_map<std::string, int> label_index;
    std::vector<int> successors;
};
} // namespace backend
