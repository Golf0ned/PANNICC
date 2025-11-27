#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class Tile {
    public:
        Tile(uint64_t cost, lir::OperandManager *om);
        uint64_t getCost();
        lir::Operand *resolveOperand(Node *node, std::vector<Node *> &worklist);
        lir::Operand *resolveOperand(AddressNode *node,
                                     std::vector<Node *> &worklist);
        lir::Operand *resolveOperand(RegisterNode *node,
                                     std::vector<Node *> &worklist);
        virtual bool matches(Node *root) = 0;
        virtual std::list<std::unique_ptr<lir::Instruction>>
        apply(std::vector<Node *> &worklist) = 0;
        virtual ~Tile() = default;

    protected:
        lir::OperandManager *om;

    private:
        uint64_t cost;
    };

    class StoreTile : public Tile {
    public:
        StoreTile(lir::OperandManager *om);
        bool matches(Node *root) override;
        std::list<std::unique_ptr<lir::Instruction>>
        apply(std::vector<Node *> &worklist) override;

    private:
        AddressNode *tile_ptr;
        Node *tile_src;
    };

    class LoadTile : public Tile {
    public:
        LoadTile(lir::OperandManager *om);
        bool matches(Node *root) override;
        std::list<std::unique_ptr<lir::Instruction>>
        apply(std::vector<Node *> &worklist) override;

    private:
        RegisterNode *tile_dst;
        LoadNode *tile_load;
    };

    class BinOpTile : public Tile {
    public:
        BinOpTile(lir::OperandManager *om);
        bool matches(Node *root) override;
        std::list<std::unique_ptr<lir::Instruction>>
        apply(std::vector<Node *> &worklist) override;

    private:
        RegisterNode *tile_dst;
        OpNode *tile_op;
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
