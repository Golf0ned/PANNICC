#include "backend/lir_tree/tile_trees.h"

namespace backend::lir_tree {
    Tile::Tile(uint64_t cost, lir::OperandManager *om) : cost(cost), om(om) {}

    uint64_t Tile::getCost() { return cost; }

    StoreTile::StoreTile(lir::OperandManager *om) : Tile(10, om) {}

    bool StoreTile::matches(Node *root) {
        auto store = dynamic_cast<StoreNode *>(root);
        if (!store)
            return false;

        tile_ptr = dynamic_cast<AddressNode *>(store->getPtr().get());
        tile_src = dynamic_cast<RegisterNode *>(store->getSource().get());
        return tile_ptr && tile_src;
    }

    std::list<std::unique_ptr<lir::Instruction>>
    StoreTile::apply(Node *root, std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto &base_ptr = tile_ptr->getBase();
        auto base = base_ptr ? om->getRegister(base_ptr->getName()) : nullptr;
        if (base_ptr)
            worklist.push_back(base_ptr.get());

        auto &index_ptr = tile_ptr->getIndex();
        auto index =
            index_ptr ? om->getRegister(index_ptr->getName()) : nullptr;
        if (index_ptr)
            worklist.push_back(index_ptr.get());

        auto scale = om->getImmediate(tile_ptr->getScale());

        auto displacement = om->getImmediate(tile_ptr->getDisplacement());

        auto dst = om->getAddress(base, index, scale, displacement);

        auto &src_source = tile_src->getSource();
        if (src_source)
            worklist.push_back(src_source.get());

        auto src = om->getRegister(tile_src->getName());

        auto size = lir::DataSize::QUADWORD;
        auto store_asm = std::make_unique<lir::InstructionMov>(
            lir::Extend::NONE, size, size, src, dst);
        assembly.push_back(std::move(store_asm));

        return assembly;
    }

    BinOpTile::BinOpTile(lir::OperandManager *om) : Tile(10, om) {}

    bool BinOpTile::matches(Node *root) {
        auto reg = dynamic_cast<RegisterNode *>(root);
        if (!reg)
            return false;

        auto &reg_src = reg->getSource();
        if (!reg_src)
            return false;

        tile_op = dynamic_cast<OpNode *>(reg_src.get());
        return tile_op;
    }

    TreeTiler::TreeTiler(lir::OperandManager *om) : om(om) {
        all_tiles.push_back(std::make_unique<StoreTile>(om));
        all_tiles.push_back(std::make_unique<BinOpTile>(om));
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

        std::list<std::unique_ptr<lir::Instruction>> tree_instructions;

        std::vector<Node *> worklist = {tree};
        while (!worklist.empty()) {
            auto cur = worklist.back();
            worklist.pop_back();

            // TODO: is this how we want to compute cost?
            Tile *cur_tile = nullptr;
            uint64_t min_cost = -1;
            for (auto &tile : all_tiles) {
                auto tile_cost = tile->getCost();
                if (tile_cost < min_cost && tile->matches(cur)) {
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

            tree_instructions.splice(tree_instructions.begin(),
                                     cur_tile->apply(cur, worklist));
        }

        assembly.splice(assembly.end(), tree_instructions);
    }
} // namespace backend::lir_tree
