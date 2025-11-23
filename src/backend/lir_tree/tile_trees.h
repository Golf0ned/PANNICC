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

    class Tile {
    public:
        uint64_t getCost();
        virtual bool matches(Node *root) = 0;
        virtual std::list<std::unique_ptr<lir::Instruction>>
        toAsm(Node *root) = 0;
        virtual std::list<Node *> getRemaining(Node *root) = 0;

    private:
        uint64_t cost;
    };

    class StoreTile : public Tile {};
} // namespace backend::lir_tree
