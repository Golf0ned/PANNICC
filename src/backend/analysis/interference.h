#pragma once

#include "backend/analysis/liveness.h"

namespace backend {
    class Interference {
    public:
        Interference(lir::Program &p);
        void addRegister(lir::Register *reg);
        void addEdge(lir::Register *first, lir::Register *second);
        void computeInterference(Liveness &liveness);
        void printInterference();

    private:
        lir::Program &program;
        std::unordered_map<lir::Register *, size_t> reg_to_index;
        std::vector<std::vector<bool>> adj_matrix;
    };
} // namespace backend
