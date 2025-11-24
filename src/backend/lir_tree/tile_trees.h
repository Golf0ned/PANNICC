#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class Tile {
    public:
        Tile(uint64_t cost, lir::OperandManager *om);
        uint64_t getCost();
        virtual bool matches(Node *root) = 0;
        virtual std::list<std::unique_ptr<lir::Instruction>>
        toAsm(Node *root) = 0;
        virtual std::list<Node *> getRemaining(Node *root) = 0;
        virtual ~Tile() = default;

    protected:
        lir::OperandManager *om;

    private:
        uint64_t cost;
    };

    class StoreTile : public Tile {
    public:
        StoreTile(lir::OperandManager *om);
        virtual bool matches(Node *root);
        virtual std::list<std::unique_ptr<lir::Instruction>> toAsm(Node *root);
        virtual std::list<Node *> getRemaining(Node *root);
    };

    class TreeTiler {
    public:
        TreeTiler(lir::OperandManager *om);

        std::list<std::unique_ptr<lir::Instruction>> getResult();

        void tile(Node *tree);

    private:
        lir::OperandManager *om;
        std::vector<std::unique_ptr<Tile>> all_tiles;
        std::list<std::unique_ptr<lir::Instruction>> assembly;
    };
} // namespace backend::lir_tree
