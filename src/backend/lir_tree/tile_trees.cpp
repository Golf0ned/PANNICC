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

    Tile::Tile(uint64_t cost, lir::OperandManager *om) : cost(cost), om(om) {}

    uint64_t Tile::getCost() { return cost; }

    StoreTile::StoreTile(lir::OperandManager *om) : Tile(10, om) {}

    bool StoreTile::matches(Node *root) {
        auto store = dynamic_cast<StoreNode *>(root);
        if (!store) {
            return false;
        }

        auto store_ptr = dynamic_cast<AddressNode *>(store->getPtr().get());
        auto store_src = dynamic_cast<RegisterNode *>(store->getSource().get());
        return store_ptr && store_src;
    }

    std::list<std::unique_ptr<lir::Instruction>> StoreTile::toAsm(Node *root) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;
        auto store = dynamic_cast<StoreNode *>(root);

        auto store_ptr = dynamic_cast<AddressNode *>(store->getPtr().get());
        auto &ptr_base = store_ptr->getBase();
        auto base = ptr_base ? om->getRegister(ptr_base->getName()) : nullptr;
        auto &ptr_index = store_ptr->getIndex();
        auto index =
            ptr_index ? om->getRegister(ptr_index->getName()) : nullptr;
        auto scale = om->getImmediate(store_ptr->getScale());
        auto displacement = om->getImmediate(store_ptr->getDisplacement());

        auto store_source =
            dynamic_cast<RegisterNode *>(store->getSource().get());

        auto size = lir::DataSize::QUADWORD;
        auto dst = om->getAddress(base, index, scale, displacement);
        auto src = om->getRegister(store_source->getName());
        auto store_asm = std::make_unique<lir::InstructionMov>(
            lir::Extend::NONE, size, size, src, dst);
        assembly.push_back(std::move(store_asm));

        return assembly;
    }

    std::list<Node *> StoreTile::getRemaining(Node *root) {
        std::list<Node *> remaining;
        auto store = dynamic_cast<StoreNode *>(root);

        auto store_ptr = dynamic_cast<AddressNode *>(store->getPtr().get());
        auto &ptr_base = store_ptr->getBase();
        if (ptr_base)
            remaining.push_back(ptr_base.get());
        auto &ptr_index = store_ptr->getIndex();
        if (ptr_index)
            remaining.push_back(ptr_index.get());

        auto store_src = dynamic_cast<RegisterNode *>(store->getSource().get());
        auto &src_source = store_src->getSource();
        if (src_source)
            remaining.push_back(src_source.get());
        return remaining;
    }
} // namespace backend::lir_tree
