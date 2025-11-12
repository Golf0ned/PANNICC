#include "backend/mir_to_lir.h"
#include "backend/lir_tree/gen_trees.h"
#include "backend/lir_tree/merge_trees.h"
#include "backend/lir_tree/tile_trees.h"
#include "middleend/utils/traversal.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir) {
        lir::OperandManager om;

        // Generate trees per instruction
        lir_tree::TreeGenVisitor tgv(mir, om);
        for (auto &f : mir.getFunctions()) {
            auto linearized = middleend::traverseTraces(f.get());

            tgv.startFunction(f.get());
            auto &bbs = f->getBasicBlocks();
            for (auto iter = bbs.begin(); iter != bbs.end(); iter++) {
                auto next = std::next(iter);

                auto bb = iter->get();
                auto next_bb = next == bbs.end() ? nullptr : next->get();

                tgv.startBasicBlock(bb, next_bb);
                for (auto &i : iter->get()->getInstructions())
                    i->accept(&tgv);

                bb->getTerminator()->accept(&tgv);
            }
            tgv.endFunction();
        }

        // Merge trees
        lir_tree::TreeMerger tmv;
        auto program_trees = tgv.getResult();
        for (auto &fn_trees : program_trees)
            tmv.mergeTrees(fn_trees);

        // Tile trees
        lir_tree::TreeTileVisitor ttv(om);
        auto merged_trees = tmv.getResult();
        for (auto &tree : merged_trees) {
            tree->accept(&ttv);
        }

        // Final cleanup
        auto instructions = ttv.getResult();
        lir::Program lir(std::move(instructions));
        return std::move(lir);
    }
} // namespace backend
