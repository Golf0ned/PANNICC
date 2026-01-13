#pragma once

#include "backend/analysis/liveness.h"

namespace backend {
    class Interference {
    public:
        Interference(lir::Program &p);

        void addRegister(lir::Register *reg);
        void addEdge(lir::Register *first, lir::Register *second);
        bool sameReg(lir::Register *first, lir::Register *second);
        std::vector<lir::Register *> getSizedRegisters(lir::Register *reg);
        void addAllEdges(lir::Register *first, lir::Register *second);
        void addPhysicalRegisters();

        void computeInterference(Liveness &liveness);
        void printInterference();

    private:
        lir::OperandManager *om;
        std::unordered_map<lir::Register *, size_t> reg_to_index;
        std::vector<std::vector<bool>> adj_matrix;
    };
} // namespace backend
