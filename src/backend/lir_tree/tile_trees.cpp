#include "backend/lir_tree/tile_trees.h"

namespace backend::lir_tree {
    TreeTiler::TreeTiler(lir::OperandManager &om) : om(om) {
        all_tiles.push_back(std::make_unique<StoreTile>());
    }

    std::list<std::unique_ptr<lir::Instruction>> TreeTiler::getResult() {
        return std::move(assembly);
    }

    void TreeTiler::tile(Node *tree) {
        auto asm_node = dynamic_cast<AsmNode *>(tree);
        if (asm_node) {
            assembly.splice(assembly.end(), asm_node->getAssembly());
            return;
        }

        std::list<std::unique_ptr<lir::Instruction>> instructions;

        std::list<Node *> worklist = {tree};
        while (!worklist.empty()) {
            auto cur = worklist.back();
            worklist.pop_back();

            Tile *cur_tile = nullptr;
            uint64_t min_cost = -1;
            for (auto &tile : all_tiles) {
                auto tile_cost = tile->getCost();
                if (tile->matches(cur) && tile_cost < min_cost) {
                    cur_tile = tile.get();
                    min_cost = tile_cost;
                }
            }

            // TODO: panic on nullptr
            if (!cur_tile) {
                auto unknown = std::make_unique<lir::InstructionUnknown>();
                assembly.push_back(std::move(unknown));
                continue;
            }

            instructions.splice(instructions.begin(), cur_tile->toAsm(cur));
            worklist.splice(worklist.end(), cur_tile->getRemaining(cur));
        }

        assembly.splice(assembly.end(), instructions);
    }

    Tile::Tile(uint64_t cost) : cost(cost) {}

    uint64_t Tile::getCost() { return cost; }

    StoreTile::StoreTile() : Tile(0) {}

    bool StoreTile::matches(Node *root) {
        // TODO
        return false;
    }

    std::list<std::unique_ptr<lir::Instruction>> StoreTile::toAsm(Node *root) {
        // TODO
        return {};
    }

    std::list<Node *> StoreTile::getRemaining(Node *root) {
        // TODO
        return {};
    }
} // namespace backend::lir_tree
