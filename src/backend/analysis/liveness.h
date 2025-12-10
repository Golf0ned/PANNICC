#pragma once

#include <unordered_set>

#include "backend/lir/lir.h"

namespace backend {
    using RegisterSet = std::unordered_set<lir::Register *>;

    class GenSetVisitor : public lir::InstructionVisitor {
    public:
        RegisterSet getResult();

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

        void visit(lir::InstructionPhi *i) override;
        void visit(lir::InstructionVirtual *i) override;
        void visit(lir::InstructionUnknown *i) override;

    private:
        RegisterSet gen;
    };

    class KillSetVisitor : public lir::InstructionVisitor {
    public:
        RegisterSet getResult();

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

        void visit(lir::InstructionPhi *i) override;
        void visit(lir::InstructionVirtual *i) override;
        void visit(lir::InstructionUnknown *i) override;

    private:
        RegisterSet kill;
    };

    class Liveness {
    public:
        void computeLiveRanges(lir::Program p);
        std::vector<RegisterSet> getIn();
        std::vector<RegisterSet> getOut();

    private:
        std::vector<RegisterSet> in;
        std::vector<RegisterSet> out;
        GenSetVisitor gsv;
        KillSetVisitor ksv;
    };
} // namespace backend
