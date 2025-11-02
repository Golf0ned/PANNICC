#include "backend/mir_to_lir.h"
#include "backend/lir_tree/gen_trees.h"
#include "backend/lir_tree/tile_trees.h"
#include "middleend/utils/traversal.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir) {
        // Generate trees per instruction
        lir::OperandManager om;
        lir_tree::TreeGenVisitor tree_gen(mir, om);
        for (auto &f : mir.getFunctions()) {
            auto linearized = middleend::traverseTraces(f.get());

            tree_gen.startFunction(f.get());
            auto &bbs = f->getBasicBlocks();
            for (auto iter = bbs.begin(); iter != bbs.end(); iter++) {
                auto next = std::next(iter);

                auto bb = iter->get();
                auto next_bb = next == bbs.end() ? nullptr : next->get();

                tree_gen.startBasicBlock(bb, next_bb);
                for (auto &i : iter->get()->getInstructions())
                    i->accept(&tree_gen);
            }
            tree_gen.endFunction();
        }

        auto program_trees = tree_gen.getResult();

        // Merge trees
        for (auto &function_trees : program_trees) {
            // TODO: merge trees
        }

        // Tile trees
        lir_tree::TreeTileVisitor tile_tree(om);
        for (auto &function_trees : program_trees) {
            for (auto &tree : function_trees) {
                tree->accept(&tile_tree);
            }
        }
        auto instructions = tile_tree.getResult();

        // Final cleanup
        lir::Program lir(std::move(instructions));
        return std::move(lir);
    }
} // namespace backend
