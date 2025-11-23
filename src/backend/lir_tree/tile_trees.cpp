#include "backend/lir_tree/tile_trees.h"

namespace backend::lir_tree {
    std::vector<Tile> tiles = {
        StoreTile(),
    };

    TreeTiler::TreeTiler(lir::OperandManager &om) : om(om) {}

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
            for (auto &tile : tiles) {
                auto tile_cost = tile.getCost();
                if (tile.matches(cur) && tile_cost < min_cost) {
                    cur_tile = &tile;
                    min_cost = tile_cost;
                }
            }

            instructions.splice(instructions.begin(), cur_tile->toAsm(cur));
            worklist.splice(worklist.end(), cur_tile->getRemaining(cur));
        }

        assembly.splice(assembly.end(), instructions);
    }
} // namespace backend::lir_tree
