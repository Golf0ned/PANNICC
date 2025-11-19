#include "backend/lir_tree/tile_trees.h"

namespace backend::lir_tree {
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

        auto reg_node = dynamic_cast<AsmNode *>(tree);
        if (reg_node) {
        }

        auto store_node = dynamic_cast<AsmNode *>(tree);
        if (store_node) {
        }

        auto unknown = std::make_unique<lir::InstructionUnknown>();
        assembly.push_back(std::move(unknown));
    }
} // namespace backend::lir_tree
