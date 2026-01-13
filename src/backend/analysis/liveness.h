#pragma once

#include <unordered_set>

#include "backend/lir/lir.h"

namespace backend {
    class GenSetVisitor : public lir::InstructionVisitor {
    public:
        GenSetVisitor(lir::OperandManager *om);

        std::unordered_set<lir::Register *> getResult();
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

        void visit(lir::InstructionVirtualCall *i) override;
        void visit(lir::InstructionUnknown *i) override;

    private:
        std::unordered_set<lir::Register *> gen;
        lir::OperandManager *om;
    };

    class KillSetVisitor : public lir::InstructionVisitor {
    public:
        KillSetVisitor(lir::OperandManager *om);

        std::unordered_set<lir::Register *> getResult();
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

        void visit(lir::InstructionVirtualCall *i) override;
        void visit(lir::InstructionUnknown *i) override;

    private:
        std::unordered_set<lir::Register *> kill;
        lir::OperandManager *om;
    };

    class SuccessorVisitor : public lir::InstructionVisitor {
    public:
        SuccessorVisitor(
            std::list<std::unique_ptr<lir::Instruction>> &instructions);

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

        void visit(lir::InstructionVirtualCall *i) override;
        void visit(lir::InstructionUnknown *i) override;

    private:
        std::unordered_map<lir::Instruction *, int> next_index;
        std::unordered_map<std::string, int> label_index;
        std::vector<int> successors;
    };

    class Liveness {
    public:
        Liveness(lir::Program &p);
        void computeLiveRanges();
        void printLiveness();
        std::vector<std::vector<lir::Register *>> getGen();
        std::vector<std::vector<lir::Register *>> getKill();
        std::vector<std::vector<lir::Register *>> getIn();
        std::vector<std::vector<lir::Register *>> getOut();

    private:
        std::vector<std::unordered_set<lir::Register *>> gen;
        std::vector<std::unordered_set<lir::Register *>> kill;
        std::vector<std::unordered_set<lir::Register *>> in;
        std::vector<std::unordered_set<lir::Register *>> out;
        lir::Program &program;
        GenSetVisitor gsv;
        KillSetVisitor ksv;
        SuccessorVisitor sv;
    };
} // namespace backend
