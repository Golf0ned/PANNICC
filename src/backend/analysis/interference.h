#pragma once

#include "backend/analysis/liveness.h"

namespace backend {
    using Interference = std::unordered_map<lir::Register *, RegisterSet>;

    Interference computeInterference(lir::Program &p, Liveness &l);
    void printInterference(Interference &i);

    class InterferenceBuilder {
    public:
        InterferenceBuilder(lir::OperandManager *om);

        Interference getInterference();

        bool sameReg(lir::Register *first, lir::Register *second);
        std::vector<lir::Register *> getSizedRegisters(lir::Register *reg);
        void interfere(lir::Register *first, lir::Register *second);

    private:
        lir::OperandManager *om;
        Interference interference;
    };
} // namespace backend
