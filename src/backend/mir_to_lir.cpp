#include "backend/mir_to_lir.h"
#include "backend/lir_tree/gen_trees.h"
#include "middleend/utils/traversal.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir) {
        // Generate trees per instruction
        lir::OperandManager om;
        lir_tree::TreeGenVisitor visitor(mir, om);
        for (auto &f : mir.getFunctions()) {
            auto linearized = middleend::traverseTraces(f.get());

            visitor.startFunction(f.get());
            auto &bbs = f->getBasicBlocks();
            for (auto iter = bbs.begin(); iter != bbs.end(); iter++) {
                auto next = std::next(iter);

                auto bb = iter->get();
                auto next_bb = next == bbs.end() ? nullptr : next->get();

                visitor.startBasicBlock(bb, next_bb);
                for (auto &i : iter->get()->getInstructions())
                    i->accept(&visitor);
            }
            visitor.endFunction();
        }

        auto program_trees = visitor.getResult();

        // Merge trees
        for (auto &function_trees : program_trees) {
            // TODO: merge trees
        }

        // Tile trees
        std::list<std::unique_ptr<lir::Instruction>> instructions;
        for (auto &function_trees : program_trees) {
            for (auto &tree : function_trees) {
                // TODO: tile trees
            }
        }

        // Final cleanup
        lir::Program lir(std::move(instructions));
        return std::move(lir);
    }

} // namespace backend
