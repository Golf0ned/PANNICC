#include "backend/lir_tree/tile_trees.h"

namespace backend::lir_tree {
    Tile::Tile(lir::OperandManager *om) : om(om) {}

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

    StoreTile::StoreTile(lir::OperandManager *om) : Tile(om) {}

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

    LoadTile::LoadTile(lir::OperandManager *om) : Tile(om) {}

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

    BinOpTile::BinOpTile(lir::OperandManager *om) : Tile(om) {}

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

            std::list<lir::Operand *> in_src_list = {left};
            auto in_asm = std::make_unique<lir::InstructionPhi>(
                std::move(in_src_list), eax);
            assembly.push_back(std::move(in_asm));

            auto extend_asm =
                std::make_unique<lir::InstructionConvert>(size, size);
            assembly.push_back(std::move(extend_asm));

            auto div_asm = std::make_unique<lir::InstructionSpecialOp>(
                bin_op, size, right);
            assembly.push_back(std::move(div_asm));

            std::list<lir::Operand *> out_src_list = {eax};
            auto out_asm = std::make_unique<lir::InstructionPhi>(
                std::move(out_src_list), dst);
            assembly.push_back(std::move(out_asm));
        } else {
            std::list<lir::Operand *> phi_list = {left};
            if (op == middleend::mir::BinaryOp::SHL ||
                op == middleend::mir::BinaryOp::ASHR) {
                phi_list.push_back(om->getRegister(lir::RegisterNum::RCX));
            }

            auto phi_asm =
                std::make_unique<lir::InstructionPhi>(std::move(phi_list), dst);
            assembly.push_back(std::move(phi_asm));

            auto op_asm = std::make_unique<lir::InstructionBinaryOp>(
                bin_op, size, right, dst);
            assembly.push_back(std::move(op_asm));
        }

        return assembly;
    }

    LeaBISDTile::LeaBISDTile(lir::OperandManager *om) : Tile(om) {}

    bool LeaBISDTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        auto &dst_source = tile_dst->getSource();
        if (!dst_source)
            return false;

        auto op = dynamic_cast<OpNode *>(dst_source.get());
        if (!op || op->getOp() != middleend::mir::BinaryOp::ADD)
            return false;

        auto left_reg = dynamic_cast<RegisterNode *>(op->getLeft().get());
        auto right_reg = dynamic_cast<RegisterNode *>(op->getRight().get());
        if (!left_reg && !right_reg)
            return false;

        //
        //   +
        //  / \
        // $   +
        //    / \   (with reorderings)
        //   *   %
        //  / \
        // $   %
        //
        if (!left_reg || !right_reg) {
            tile_displacement = static_cast<ImmediateNode *>(
                left_reg ? op->getRight().get() : op->getLeft().get());
            auto inner_reg = left_reg ? left_reg : right_reg;

            auto &inner_reg_source = inner_reg->getSource();
            if (!inner_reg_source)
                return false;

            auto inner_op = dynamic_cast<OpNode *>(inner_reg_source.get());
            if (!inner_op || inner_op->getOp() != middleend::mir::BinaryOp::ADD)
                return false;

            auto inner_left =
                dynamic_cast<RegisterNode *>(inner_op->getLeft().get());
            auto inner_right =
                dynamic_cast<RegisterNode *>(inner_op->getRight().get());
            if (!inner_left || !inner_right)
                return false;

            if (matchScaledIndex(inner_left))
                tile_base = inner_right;
            else if (matchScaledIndex(inner_right))
                tile_base = inner_left;
            else
                return false;

            return true;
        }

        //
        //   +
        //  / \              +
        // %   +           /   \
        //    / \   or    *     +    (with reorderings)
        //   *   $       / \   / \
        //  / \         $   % $   %
        // $   %
        //
        auto &left_reg_source = left_reg->getSource();
        auto &right_reg_source = right_reg->getSource();

        auto left_op = left_reg_source
                           ? dynamic_cast<OpNode *>(left_reg_source.get())
                           : nullptr;
        auto right_op = right_reg_source
                            ? dynamic_cast<OpNode *>(right_reg_source.get())
                            : nullptr;

        auto matchDoubleReg = [&](OpNode *op_node, RegisterNode *reg_node) {
            if (!op_node || op_node->getOp() != middleend::mir::BinaryOp::ADD)
                return false;

            auto op_left_imm =
                dynamic_cast<ImmediateNode *>(op_node->getLeft().get());
            auto op_right_imm =
                dynamic_cast<ImmediateNode *>(op_node->getRight().get());

            if (!!op_left_imm ^ !op_right_imm)
                return false;

            tile_displacement = op_left_imm ? op_left_imm : op_right_imm;
            auto other_child = static_cast<RegisterNode *>(
                op_left_imm ? op_node->getRight().get()
                            : op_node->getLeft().get());

            if (matchScaledIndex(other_child)) {
                tile_base = reg_node;
                return true;
            }

            if (matchScaledIndex(reg_node)) {
                tile_base = other_child;
                return true;
            }

            return false;
        };

        return matchDoubleReg(left_op, right_reg) ||
               matchDoubleReg(right_op, left_reg);
    }

    bool LeaBISDTile::matchScaledIndex(RegisterNode *node) {
        auto &reg_src = node->getSource();
        if (!reg_src)
            return false;

        auto op = dynamic_cast<OpNode *>(reg_src.get());
        if (!op)
            return false;

        if (op->getOp() != middleend::mir::BinaryOp::MUL)
            return false;

        auto left_reg = dynamic_cast<RegisterNode *>(op->getLeft().get());
        auto right_reg = dynamic_cast<RegisterNode *>(op->getRight().get());
        // TODO: what
        if (!left_reg == !right_reg)
            return false;

        auto index = left_reg ? left_reg : right_reg;

        auto scale = dynamic_cast<ImmediateNode *>(
            left_reg ? op->getRight().get() : op->getLeft().get());
        if (!scale)
            return false;

        switch (scale->getValue()) {
        case 1:
        case 2:
        case 4:
        case 8:
            break;
        default:
            return false;
        }

        tile_index = index;
        tile_scale = scale;
        return true;
    }

    std::list<std::unique_ptr<lir::Instruction>>
    LeaBISDTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;

        auto base = resolveOperand(tile_base, worklist);
        auto index = resolveOperand(tile_index, worklist);
        auto scale = om->getImmediate(tile_scale->getValue());
        auto displacement = om->getImmediate(tile_displacement->getValue());
        auto src = om->getAddress(static_cast<lir::Register *>(base),
                                  static_cast<lir::Register *>(index), scale,
                                  displacement);

        auto dst = om->getRegister(tile_dst->getName());

        auto lea = std::make_unique<lir::InstructionLea>(size, src, dst);
        assembly.push_back(std::move(lea));

        return std::move(assembly);
    }

    TreeTiler::TreeTiler(lir::OperandManager *om) : om(om) {
        // lea tiles
        all_tiles.push_back(std::make_unique<LeaBISDTile>(om));

        // Atomic tiles
        all_tiles.push_back(std::make_unique<BinOpTile>(om));
        all_tiles.push_back(std::make_unique<StoreTile>(om));
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

            Tile *cur_tile = nullptr;
            for (auto &tile : all_tiles) {
                if (tile->matches(cur)) {
                    cur_tile = tile.get();
                    break;
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
