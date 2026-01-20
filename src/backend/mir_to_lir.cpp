#include "backend/mir_to_lir.h"
#include "backend/lir_tree/gen_trees.h"
#include "backend/lir_tree/merge_trees.h"
#include "backend/lir_tree/tile_trees.h"
#include "middleend/transform/insert_parallel_copies.h"
#include "middleend/transform/split_critical.h"
#include "middleend/utils/traversal.h"

namespace backend {
    using FunctionTrees = std::list<std::unique_ptr<lir_tree::Node>>;

    std::tuple<std::list<FunctionTrees>, std::unique_ptr<lir_tree::TreeInfo>,
               std::vector<std::unique_ptr<lir_tree::FunctionInfo>>>
    generateTrees(middleend::mir::Program &mir, lir::OperandManager *om) {
        lir_tree::TreeGenVisitor tgv(mir, om);

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

        return {tgv.getResult(), tgv.getInfo()};
    }

    void mergeTrees(std::list<FunctionTrees> &trees,
                    lir_tree::TreeInfo *tree_info, lir::OperandManager *om) {
        lir_tree::TreeMerger merger;
        std::list<std::list<std::unique_ptr<lir_tree::Node>>> merged_trees = {};
        for (auto &fn_trees : trees) {
            merger.mergeTrees(fn_trees, om);
            merged_trees.push_back(merger.getResult());
        }

        return std::move(merged_trees);
    }

    std::list<std::unique_ptr<lir::Function>> tileTrees(
        std::list<FunctionTrees> &trees,
        std::vector<std::unique_ptr<lir_tree::FunctionInfo>> &function_info,
        lir::OperandManager *om) {
        lir_tree::TreeTiler tiler(om);
        std::list<std::unique_ptr<lir::Function>> functions;
        auto f_trees = trees.begin();
        for (auto &f_info : function_info) {
            tiler.reset();
            for (auto &tree : *f_trees++)
                tiler.tile(tree.get());

            auto name = f_info->getName();
            auto num_params = f_info->getNumParams();
            auto stack_bytes = f_info->getStackBytes();
            auto instructions = std::move(tiler.getResult());

            auto function = std::make_unique<lir::Function>(
                name, num_params, stack_bytes, std::move(instructions));
            functions.push_back(std::move(function));
        }

        return std::move(functions);
    }

    lir::Program mirToLir(middleend::mir::Program &mir) {
        auto om = std::make_unique<lir::OperandManager>();
        bool merge = true;

        // Construct conventional SSA + begin SSA destruction
        middleend::SplitCritical sc;
        sc.run(mir);
        middleend::InsertParallelCopies ipc;
        ipc.run(mir);

        // Perform isel
        auto [trees, tree_info, function_info] = generateTrees(mir, om.get());
        if (merge)
            mergeTrees(trees, tree_info.get(), om.get());
        auto functions = tileTrees(trees, function_info, om.get());

        // Final cleanup
        lir::Program lir(std::move(functions), std::move(om));
        return std::move(lir);
    }
} // namespace backend
