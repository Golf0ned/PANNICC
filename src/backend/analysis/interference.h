#pragma once

#include "backend/analysis/liveness.h"

namespace backend {
    class Interference {
    public:
        Interference(Liveness &liveness);

    private:
        std::unordered_map<lir::Register *, size_t> reg_to_index;
        std::vector<std::vector<bool>> adj_matrix;
    };
} // namespace backend
