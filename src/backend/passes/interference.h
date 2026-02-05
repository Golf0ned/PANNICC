#pragma once

#include "backend/passes/liveness.h"

namespace backend {
    using Interference = std::unordered_map<lir::Register *, RegisterSet>;

    constexpr lir::DataSize flat_size = lir::DataSize::QUADWORD;

    Interference computeInterference(lir::Function *f, Liveness &l,
                                     lir::OperandManager *om);

    void printInterference(Interference &i);

    class InterferenceBuilder {
    public:
        InterferenceBuilder(lir::OperandManager *om);

        Interference getInterference();

        bool sameReg(lir::Register *first, lir::Register *second);
        lir::Register *flattenRegister(lir::Register *reg);
        void interfere(lir::Register *first, lir::Register *second);
        void addReg(lir::Register *reg);

    private:
        lir::OperandManager *om;
        Interference interference;
    };
} // namespace backend
