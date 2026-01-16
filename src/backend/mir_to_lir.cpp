#include "backend/mir_to_lir.h"
#include "backend/lir_tree/gen_trees.h"
#include "backend/lir_tree/merge_trees.h"
#include "backend/lir_tree/tile_trees.h"
#include "middleend/transform/insert_parallel_copies.h"
#include "middleend/transform/split_critical.h"
#include "middleend/utils/traversal.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir) {
        auto om = std::make_unique<lir::OperandManager>();

        // Construct conventional SSA + begin SSA destruction
        middleend::SplitCritical sc;
        sc.run(mir);
        middleend::InsertParallelCopies ipc;
        ipc.run(mir);

        // Generate trees per instruction
        lir_tree::TreeGenVisitor tgv(mir, om.get());
        for (auto &f : mir.getFunctions()) {
            auto linearized = middleend::traverseTraces(f.get());

            tgv.startFunction(f.get());
            for (auto iter = linearized.begin(); iter != linearized.end();
                 iter++) {
                auto next = std::next(iter);

                auto bb = *iter;
                auto next_bb = next == linearized.end() ? nullptr : *next;

                tgv.startBasicBlock(bb, next_bb);
                for (auto &i : (*iter)->getInstructions())
                    i->accept(&tgv);

                bb->getTerminator()->accept(&tgv);
            }
            tgv.endFunction();
        }

        // Merge trees
        lir_tree::TreeMerger merger;
        auto program_trees = tgv.getResult();
        std::list<std::list<std::unique_ptr<lir_tree::Node>>> merged_trees = {};
        for (auto &fn_trees : program_trees) {
            merger.mergeTrees(fn_trees, om.get());
            merged_trees.push_back(merger.getResult());
        }

        // Tile trees
        lir_tree::TreeTiler tiler(om.get());
        std::list<std::unique_ptr<lir::Function>> functions;
        for (auto &fn_trees : merged_trees) {
            tiler.reset();
            for (auto &tree : fn_trees)
                tiler.tile(tree.get());

            // TODO: preserve num params + stack bytes
            auto instructions = std::move(tiler.getResult());
            auto function =
                std::make_unique<lir::Function>(std::move(instructions), 0, 0);
            functions.push_back(std::move(function));
        }

        // Final cleanup
        lir::Program lir(std::move(functions), std::move(om));
        return std::move(lir);
    }
} // namespace backend
