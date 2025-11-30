#include "backend/lir_tree/tile_trees.h"

namespace backend::lir_tree {
    Tile::Tile(uint64_t cost, lir::OperandManager *om) : cost(cost), om(om) {}

    uint64_t Tile::getCost() { return cost; }

    lir::Operand *Tile::resolveOperand(Node *node,
                                       std::vector<Node *> &worklist) {
        auto reg = dynamic_cast<RegisterNode *>(node);
        if (reg)
            return resolveOperand(reg, worklist);

        auto immediate = dynamic_cast<ImmediateNode *>(node);
        if (immediate)
            return om->getImmediate(immediate->getValue());

        auto address = dynamic_cast<AddressNode *>(node);
        if (address)
            return resolveOperand(address, worklist);

        return nullptr;
    }

    lir::Operand *Tile::resolveOperand(AddressNode *node,
                                       std::vector<Node *> &worklist) {
        auto &base_ptr = node->getBase();
        auto base = base_ptr ? static_cast<lir::Register *>(
                                   resolveOperand(base_ptr.get(), worklist))
                             : nullptr;

        auto &index_ptr = node->getIndex();
        auto index = index_ptr ? static_cast<lir::Register *>(
                                     resolveOperand(index_ptr.get(), worklist))
                               : nullptr;

        auto scale = om->getImmediate(node->getScale());

        auto displacement = om->getImmediate(node->getDisplacement());

        return om->getAddress(base, index, scale, displacement);
    }

    lir::Operand *Tile::resolveOperand(RegisterNode *node,
                                       std::vector<Node *> &worklist) {
        // TODO: handle non-virtual registers
        if (node->getSource())
            worklist.push_back(node);

        return om->getRegister(node->getName());
    }

    StoreTile::StoreTile(lir::OperandManager *om) : Tile(10, om) {}

    bool StoreTile::matches(Node *root) {
        auto store = dynamic_cast<StoreNode *>(root);
        if (!store)
            return false;

        tile_ptr = dynamic_cast<AddressNode *>(store->getPtr().get());
        tile_src = store->getSource().get();
        return tile_ptr && tile_src;
    }

    std::list<std::unique_ptr<lir::Instruction>>
    StoreTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;
        auto src = resolveOperand(tile_src, worklist);
        auto dst = resolveOperand(tile_ptr, worklist);

        auto store_asm = std::make_unique<lir::InstructionMov>(
            lir::Extend::NONE, size, size, src, dst);
        assembly.push_back(std::move(store_asm));

        return assembly;
    }

    LoadTile::LoadTile(lir::OperandManager *om) : Tile(10, om) {}

    bool LoadTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        auto &reg_src = tile_dst->getSource();
        if (!reg_src)
            return false;

        tile_load = dynamic_cast<LoadNode *>(reg_src.get());
        return tile_load;
    }

    std::list<std::unique_ptr<lir::Instruction>>
    LoadTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;
        auto src = resolveOperand(tile_load->getPtr().get(), worklist);
        auto dst = om->getRegister(tile_dst->getName());

        auto load_asm = std::make_unique<lir::InstructionMov>(
            lir::Extend::NONE, size, size, src, dst);
        assembly.push_back(std::move(load_asm));

        return assembly;
    }

    BinOpTile::BinOpTile(lir::OperandManager *om) : Tile(10, om) {}

    bool BinOpTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        auto &reg_src = tile_dst->getSource();
        if (!reg_src)
            return false;

        tile_op = dynamic_cast<OpNode *>(reg_src.get());
        return tile_op;
    }

    std::list<std::unique_ptr<lir::Instruction>>
    BinOpTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto left = resolveOperand(tile_op->getLeft().get(), worklist);
        auto right = resolveOperand(tile_op->getRight().get(), worklist);
        auto dst = om->getRegister(tile_dst->getName());

        auto size = lir::DataSize::DOUBLEWORD;

        auto op = tile_op->getOp();
        auto bin_op = lir::fromMir(op);
        if (op == middleend::mir::BinaryOp::SDIV) {
            auto eax = om->getRegister(lir::RegisterNum::EAX);

            auto mov_in_asm = std::make_unique<lir::InstructionMov>(
                lir::Extend::NONE, size, size, left, eax);
            assembly.push_back(std::move(mov_in_asm));

            auto extend_asm =
                std::make_unique<lir::InstructionConvert>(size, size);
            assembly.push_back(std::move(extend_asm));

            auto div_asm = std::make_unique<lir::InstructionSpecialOp>(
                bin_op, size, right);
            assembly.push_back(std::move(div_asm));

            auto mov_out_asm = std::make_unique<lir::InstructionMov>(
                lir::Extend::NONE, size, size, eax, dst);
            assembly.push_back(std::move(mov_out_asm));
        } else {
            auto mov_asm = std::make_unique<lir::InstructionMov>(
                lir::Extend::NONE, size, size, left, dst);
            assembly.push_back(std::move(mov_asm));

            auto op_asm = std::make_unique<lir::InstructionBinaryOp>(
                bin_op, size, right, dst);
            assembly.push_back(std::move(op_asm));
        }

        return assembly;
    }

    TreeTiler::TreeTiler(lir::OperandManager *om) : om(om) {
        all_tiles.push_back(std::make_unique<StoreTile>(om));
        all_tiles.push_back(std::make_unique<BinOpTile>(om));
        all_tiles.push_back(std::make_unique<LoadTile>(om));
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
                                     cur_tile->apply(worklist));
        }

        assembly.splice(assembly.end(), tree_instructions);
    }
} // namespace backend::lir_tree
