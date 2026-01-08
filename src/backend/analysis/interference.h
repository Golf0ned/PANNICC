#pragma once

#include "backend/analysis/liveness.h"

namespace backend {
    class Interference {
    public:
        Interference(lir::Program &p);
        void computeInterference(Liveness &liveness);
        void addEdge(lir::Register *first, lir::Register *second);

    private:
        lir::Program &program;
        std::unordered_map<lir::Register *, size_t> reg_to_index;
        std::vector<std::vector<bool>> adj_matrix;
    };
} // namespace backend
