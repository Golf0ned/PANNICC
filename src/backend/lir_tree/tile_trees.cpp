#include "backend/lir_tree/tile_trees.h"

namespace backend::lir_tree {
    bool matchIS(RegisterNode *node, RegisterNode **index_res,
                 ImmediateNode **scale_res) {
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

        *index_res = index;
        *scale_res = scale;
        return true;
    }

    bool matchISS(RegisterNode *node, RegisterNode **index_res,
                  ImmediateNode **scale_res) {
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
        if (!left_reg == !right_reg)
            return false;

        auto index = left_reg ? left_reg : right_reg;

        auto scale = dynamic_cast<ImmediateNode *>(
            left_reg ? op->getRight().get() : op->getLeft().get());
        if (!scale)
            return false;

        switch (scale->getValue()) {
        case 3:
        case 5:
        case 9:
            break;
        default:
            return false;
        }

        *index_res = index;
        *scale_res = scale;
        return true;
    }

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
        if (!tile_ptr)
            return false;

        tile_src = store->getSource().get();
        if (!tile_src)
            return false;

        return true;
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
        // TODO: fix multiply operands
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

    LeaBITile::LeaBITile(lir::OperandManager *om) : Tile(om) {}

    bool LeaBITile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        auto &reg_src = tile_dst->getSource();
        if (!reg_src)
            return false;

        auto op = dynamic_cast<OpNode *>(reg_src.get());
        if (!op || op->getOp() != middleend::mir::BinaryOp::ADD)
            return false;

        auto left_reg = dynamic_cast<RegisterNode *>(op->getLeft().get());
        auto right_reg = dynamic_cast<RegisterNode *>(op->getRight().get());
        if (!left_reg || !right_reg)
            return false;

        tile_base = left_reg;
        tile_index = right_reg;
        return true;
    }

    std::list<std::unique_ptr<lir::Instruction>>
    LeaBITile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;

        auto base = resolveOperand(tile_base, worklist);
        auto index = resolveOperand(tile_index, worklist);
        auto scale = om->getImmediate(1);
        auto displacement = om->getImmediate(0);
        auto src = om->getAddress(static_cast<lir::Register *>(base),
                                  static_cast<lir::Register *>(index), scale,
                                  displacement);

        auto dst = om->getRegister(tile_dst->getName());

        auto lea = std::make_unique<lir::InstructionLea>(size, src, dst);
        assembly.push_back(std::move(lea));

        return std::move(assembly);
    }

    LeaISTile::LeaISTile(lir::OperandManager *om) : Tile(om) {}

    bool LeaISTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        return matchIS(tile_dst, &tile_index, &tile_scale);
    }

    std::list<std::unique_ptr<lir::Instruction>>
    LeaISTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;

        auto base = nullptr;
        auto index = resolveOperand(tile_index, worklist);
        auto scale = om->getImmediate(tile_scale->getValue());
        auto displacement = om->getImmediate(0);
        auto src = om->getAddress(base, static_cast<lir::Register *>(index),
                                  scale, displacement);

        auto dst = om->getRegister(tile_dst->getName());

        auto lea = std::make_unique<lir::InstructionLea>(size, src, dst);
        assembly.push_back(std::move(lea));

        return std::move(assembly);
    }

    LeaIISTile::LeaIISTile(lir::OperandManager *om) : Tile(om) {}

    bool LeaIISTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        return matchISS(tile_dst, &tile_index, &tile_scale);
    }

    std::list<std::unique_ptr<lir::Instruction>>
    LeaIISTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;

        auto base =
            static_cast<lir::Register *>(resolveOperand(tile_index, worklist));
        auto index = base;
        auto scale = om->getImmediate(tile_scale->getValue() - 1);
        auto displacement = om->getImmediate(0);
        auto src = om->getAddress(base, index, scale, displacement);

        auto dst = om->getRegister(tile_dst->getName());

        auto lea = std::make_unique<lir::InstructionLea>(size, src, dst);
        assembly.push_back(std::move(lea));

        return std::move(assembly);
    }

    LeaBISTile::LeaBISTile(lir::OperandManager *om) : Tile(om) {}

    bool LeaBISTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        auto &reg_src = tile_dst->getSource();
        if (!reg_src)
            return false;

        auto op = dynamic_cast<OpNode *>(reg_src.get());
        if (!op || op->getOp() != middleend::mir::BinaryOp::ADD)
            return false;

        auto left_reg = dynamic_cast<RegisterNode *>(op->getLeft().get());
        auto right_reg = dynamic_cast<RegisterNode *>(op->getRight().get());
        if (!left_reg || !right_reg)
            return false;

        if (matchIS(left_reg, &tile_index, &tile_scale)) {
            tile_base = right_reg;
            return true;
        }
        if (matchIS(right_reg, &tile_index, &tile_scale)) {
            tile_base = left_reg;
            return true;
        }

        return false;
    }

    std::list<std::unique_ptr<lir::Instruction>>
    LeaBISTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;

        auto base = resolveOperand(tile_base, worklist);
        auto index = resolveOperand(tile_index, worklist);
        auto scale = om->getImmediate(tile_scale->getValue());
        auto displacement = om->getImmediate(0);
        auto src = om->getAddress(static_cast<lir::Register *>(base),
                                  static_cast<lir::Register *>(index), scale,
                                  displacement);

        auto dst = om->getRegister(tile_dst->getName());

        auto lea = std::make_unique<lir::InstructionLea>(size, src, dst);
        assembly.push_back(std::move(lea));

        return std::move(assembly);
    }

    LeaBIDTile::LeaBIDTile(lir::OperandManager *om) : Tile(om) {}

    bool LeaBIDTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        auto &reg_src = tile_dst->getSource();
        if (!reg_src)
            return false;

        auto op = dynamic_cast<OpNode *>(reg_src.get());
        if (!op || op->getOp() != middleend::mir::BinaryOp::ADD)
            return false;

        auto left_reg = dynamic_cast<RegisterNode *>(op->getLeft().get());
        auto right_reg = dynamic_cast<RegisterNode *>(op->getRight().get());
        if (!left_reg && !right_reg)
            return false;

        //
        //   +
        //  / \
        // $   +    (with reorderings)
        //    / \
        //   %   %
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

            tile_base = dynamic_cast<RegisterNode *>(inner_op->getLeft().get());
            if (!tile_base)
                return false;

            tile_index =
                dynamic_cast<RegisterNode *>(inner_op->getRight().get());
            if (!tile_index)
                return false;

            return true;
        }

        //
        //   +
        //  / \
        // %   +    (with reorderings)
        //    / \
        //   %   $
        //
        auto &left_reg_source = left_reg->getSource();
        auto &right_reg_source = right_reg->getSource();

        auto left_op = left_reg_source
                           ? dynamic_cast<OpNode *>(left_reg_source.get())
                           : nullptr;
        auto right_op = right_reg_source
                            ? dynamic_cast<OpNode *>(right_reg_source.get())
                            : nullptr;

        auto matchDoubleReg = [&](OpNode *op_group, RegisterNode *reg_node) {
            if (!op_group || op_group->getOp() != middleend::mir::BinaryOp::ADD)
                return false;

            auto op_left_imm =
                dynamic_cast<ImmediateNode *>(op_group->getLeft().get());
            auto op_right_imm =
                dynamic_cast<ImmediateNode *>(op_group->getRight().get());

            if (!!op_left_imm ^ !op_right_imm)
                return false;

            tile_displacement = op_left_imm ? op_left_imm : op_right_imm;
            auto other_child = static_cast<RegisterNode *>(
                op_left_imm ? op_group->getRight().get()
                            : op_group->getLeft().get());

            tile_base = other_child;
            tile_index = reg_node;
            return true;
        };

        return matchDoubleReg(left_op, right_reg) ||
               matchDoubleReg(right_op, left_reg);
    }

    std::list<std::unique_ptr<lir::Instruction>>
    LeaBIDTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;

        auto base = resolveOperand(tile_base, worklist);
        auto index = resolveOperand(tile_index, worklist);
        auto scale = om->getImmediate(1);
        auto displacement = om->getImmediate(tile_displacement->getValue());
        auto src = om->getAddress(static_cast<lir::Register *>(base),
                                  static_cast<lir::Register *>(index), scale,
                                  displacement);

        auto dst = om->getRegister(tile_dst->getName());

        auto lea = std::make_unique<lir::InstructionLea>(size, src, dst);
        assembly.push_back(std::move(lea));

        return std::move(assembly);
    }

    LeaISDTile::LeaISDTile(lir::OperandManager *om) : Tile(om) {}

    bool LeaISDTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        auto &reg_src = tile_dst->getSource();
        if (!reg_src)
            return false;

        auto op = dynamic_cast<OpNode *>(reg_src.get());
        if (!op || op->getOp() != middleend::mir::BinaryOp::ADD)
            return false;

        auto left_reg = dynamic_cast<RegisterNode *>(op->getLeft().get());
        auto right_reg = dynamic_cast<RegisterNode *>(op->getRight().get());
        if (!!left_reg ^ !right_reg)
            return false;

        if (left_reg && matchIS(left_reg, &tile_index, &tile_scale)) {
            tile_displacement =
                static_cast<ImmediateNode *>(op->getRight().get());
            return true;
        }
        if (right_reg && matchIS(right_reg, &tile_index, &tile_scale)) {
            tile_displacement =
                static_cast<ImmediateNode *>(op->getLeft().get());
            return true;
        }

        return false;
    }

    std::list<std::unique_ptr<lir::Instruction>>
    LeaISDTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;

        auto base = nullptr;
        auto index = resolveOperand(tile_index, worklist);
        auto scale = om->getImmediate(tile_scale->getValue());
        auto displacement = om->getImmediate(tile_displacement->getValue());
        auto src = om->getAddress(base, static_cast<lir::Register *>(index),
                                  scale, displacement);

        auto dst = om->getRegister(tile_dst->getName());

        auto lea = std::make_unique<lir::InstructionLea>(size, src, dst);
        assembly.push_back(std::move(lea));

        return std::move(assembly);
    }

    LeaIISDTile::LeaIISDTile(lir::OperandManager *om) : Tile(om) {}

    bool LeaIISDTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        auto &reg_src = tile_dst->getSource();
        if (!reg_src)
            return false;

        auto op = dynamic_cast<OpNode *>(reg_src.get());
        if (!op || op->getOp() != middleend::mir::BinaryOp::ADD)
            return false;

        auto left_reg = dynamic_cast<RegisterNode *>(op->getLeft().get());
        auto right_reg = dynamic_cast<RegisterNode *>(op->getRight().get());
        if (!!left_reg ^ !right_reg)
            return false;

        if (left_reg && matchISS(left_reg, &tile_index, &tile_scale)) {
            tile_displacement =
                static_cast<ImmediateNode *>(op->getRight().get());
            return true;
        }
        if (right_reg && matchISS(right_reg, &tile_index, &tile_scale)) {
            tile_displacement =
                static_cast<ImmediateNode *>(op->getLeft().get());
            return true;
        }

        return false;
    }

    std::list<std::unique_ptr<lir::Instruction>>
    LeaIISDTile::apply(std::vector<Node *> &worklist) {
        std::list<std::unique_ptr<lir::Instruction>> assembly;

        auto size = lir::DataSize::DOUBLEWORD;

        auto base =
            static_cast<lir::Register *>(resolveOperand(tile_index, worklist));
        auto index = base;
        auto scale = om->getImmediate(tile_scale->getValue() - 1);
        auto displacement = om->getImmediate(tile_displacement->getValue());
        auto src = om->getAddress(base, index, scale, displacement);

        auto dst = om->getRegister(tile_dst->getName());

        auto lea = std::make_unique<lir::InstructionLea>(size, src, dst);
        assembly.push_back(std::move(lea));

        return std::move(assembly);
    }

    LeaBISDTile::LeaBISDTile(lir::OperandManager *om) : Tile(om) {}

    bool LeaBISDTile::matches(Node *root) {
        tile_dst = dynamic_cast<RegisterNode *>(root);
        if (!tile_dst)
            return false;

        auto &reg_src = tile_dst->getSource();
        if (!reg_src)
            return false;

        auto op = dynamic_cast<OpNode *>(reg_src.get());
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

            if (matchIS(inner_left, &tile_index, &tile_scale)) {
                tile_base = inner_right;
                return true;
            }
            if (matchIS(inner_right, &tile_index, &tile_scale)) {
                tile_base = inner_left;
                return true;
            }

            return false;
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

        auto matchDoubleReg = [&](OpNode *op_group, RegisterNode *reg_node) {
            if (!op_group || op_group->getOp() != middleend::mir::BinaryOp::ADD)
                return false;

            auto op_left_imm =
                dynamic_cast<ImmediateNode *>(op_group->getLeft().get());
            auto op_right_imm =
                dynamic_cast<ImmediateNode *>(op_group->getRight().get());

            if (!!op_left_imm ^ !op_right_imm)
                return false;

            tile_displacement = op_left_imm ? op_left_imm : op_right_imm;
            auto other_child = static_cast<RegisterNode *>(
                op_left_imm ? op_group->getRight().get()
                            : op_group->getLeft().get());

            if (matchIS(other_child, &tile_index, &tile_scale)) {
                tile_base = reg_node;
                return true;
            }
            if (matchIS(reg_node, &tile_index, &tile_scale)) {
                tile_base = other_child;
                return true;
            }

            return false;
        };

        return matchDoubleReg(left_op, right_reg) ||
               matchDoubleReg(right_op, left_reg);
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
        all_tiles.push_back(std::make_unique<LeaBISTile>(om));
        all_tiles.push_back(std::make_unique<LeaIISDTile>(om));
        all_tiles.push_back(std::make_unique<LeaISDTile>(om));
        all_tiles.push_back(std::make_unique<LeaBIDTile>(om));
        all_tiles.push_back(std::make_unique<LeaIISTile>(om));
        all_tiles.push_back(std::make_unique<LeaISTile>(om));
        all_tiles.push_back(std::make_unique<LeaBITile>(om));

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
