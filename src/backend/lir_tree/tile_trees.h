#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class TreeTiler {
    public:
        TreeTiler(lir::OperandManager &om);

        std::list<std::unique_ptr<lir::Instruction>> getResult();

        void tile(Node *tree);

    private:
        lir::OperandManager &om;
        std::list<std::unique_ptr<lir::Instruction>> assembly;
    };
} // namespace backend::lir_tree
