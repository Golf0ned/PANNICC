#include <iostream>

#include "backend/lir/operand.h"
#include "backend/passes/coloring.h"

namespace backend {
    PrecoloringVisitor::PrecoloringVisitor(lir::OperandManager *om) : om(om) {}

    // TODO: fill the rest of this in, im lazy rn
    RegisterColoring PrecoloringVisitor::getResult() { return precolorings; }

    void PrecoloringVisitor::checkConstrained(lir::Operand *operand) {
        auto constrained = dynamic_cast<lir::ConstrainedRegister *>(operand);
        if (!constrained)
            return;

        auto color = lir::toSized(constrained->getConstraint(), color_size);

        auto physical_reg = om->getRegister(color);
        auto virtual_reg = om->getRegister(constrained->getName(), color_size);

        precolorings[physical_reg] = color;
        precolorings[virtual_reg] = color;
    }

    void PrecoloringVisitor::visit(lir::Instruction *i) {}

    void PrecoloringVisitor::visit(lir::Label *l) {}

    void PrecoloringVisitor::visit(lir::InstructionMov *i) {}

    void PrecoloringVisitor::visit(lir::InstructionPush *i) {}

    void PrecoloringVisitor::visit(lir::InstructionPop *i) {}

    void PrecoloringVisitor::visit(lir::InstructionConvert *i) {}

    void PrecoloringVisitor::visit(lir::InstructionBinaryOp *i) {
        checkConstrained(i->getSrc());
        checkConstrained(i->getDst());
    }

    void PrecoloringVisitor::visit(lir::InstructionSpecialOp *i) {}

    void PrecoloringVisitor::visit(lir::InstructionLea *i) {}

    void PrecoloringVisitor::visit(lir::InstructionCmp *i) {}

    void PrecoloringVisitor::visit(lir::InstructionJmp *i) {}

    void PrecoloringVisitor::visit(lir::InstructionCJmp *i) {}

    void PrecoloringVisitor::visit(lir::InstructionCall *i) {}

    void PrecoloringVisitor::visit(lir::InstructionRet *i) {}

    void PrecoloringVisitor::visit(lir::InstructionVirtualCall *i) {}

    void PrecoloringVisitor::visit(lir::InstructionUnknown *i) {}

    AssignmentVisitor::AssignmentVisitor(const RegisterColoring &coloring,
                                         lir::OperandManager *om)
        : coloring(coloring), om(om) {}

    lir::Register *AssignmentVisitor::assignRegister(lir::Register *reg) {
        // TODO: do we care about replacing physical?
        if (reg->getRegNum() != lir::RegisterNum::VIRTUAL)
            return reg;

        auto virtual_reg = static_cast<lir::VirtualRegister *>(reg);
        auto flattened_reg =
            om->getRegister(virtual_reg->getName(), color_size);
        auto color = coloring.at(flattened_reg);
        return om->getRegister(lir::toSized(color, virtual_reg->getSize()));
    }

    lir::Operand *AssignmentVisitor::tryColorOperand(lir::Operand *operand) {
        auto reg = dynamic_cast<lir::VirtualRegister *>(operand);
        if (reg)
            return assignRegister(reg);

        auto address = dynamic_cast<lir::Address *>(operand);
        if (address) {
            auto base = address->getBase(), index = address->getIndex();
            auto new_base = base ? assignRegister(base) : nullptr;
            auto new_index = index ? assignRegister(index) : nullptr;
            return om->getAddress(new_base, new_index, address->getScale(),
                                  address->getDisplacement());
        }

        return nullptr;
    }

    void AssignmentVisitor::visit(lir::Instruction *i) {}

    void AssignmentVisitor::visit(lir::Label *l) {}

    void AssignmentVisitor::visit(lir::InstructionMov *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(src);

        auto dst = tryColorOperand(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void AssignmentVisitor::visit(lir::InstructionPush *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(src);
    }

    void AssignmentVisitor::visit(lir::InstructionPop *i) {
        auto dst = tryColorOperand(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void AssignmentVisitor::visit(lir::InstructionConvert *i) {}

    void AssignmentVisitor::visit(lir::InstructionBinaryOp *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(src);

        auto dst = tryColorOperand(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void AssignmentVisitor::visit(lir::InstructionSpecialOp *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(src);
    }

    void AssignmentVisitor::visit(lir::InstructionLea *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(static_cast<lir::Address *>(src));

        auto dst = tryColorOperand(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void AssignmentVisitor::visit(lir::InstructionCmp *i) {
        auto src1 = tryColorOperand(i->getSrc1());
        if (src1)
            i->setSrc1(src1);

        auto src2 = tryColorOperand(i->getSrc2());
        if (src2)
            i->setSrc2(src2);
    }

    void AssignmentVisitor::visit(lir::InstructionJmp *i) {}

    void AssignmentVisitor::visit(lir::InstructionCJmp *i) {}

    void AssignmentVisitor::visit(lir::InstructionCall *i) {}

    void AssignmentVisitor::visit(lir::InstructionRet *i) {}

    void AssignmentVisitor::visit(lir::InstructionVirtualCall *i) {
        auto args = i->getArgs();

        for (auto &arg : args) {
            auto colored = tryColorOperand(arg);
            if (colored)
                arg = colored;
        }

        i->setArgs(args);
    }

    void AssignmentVisitor::visit(lir::InstructionUnknown *i) {}

    RegisterColoring getPrecoloring(lir::Function *f, lir::OperandManager *om) {
        PrecoloringVisitor pcv(om);
        for (auto &i : f->getInstructions())
            i->accept(&pcv);

        return pcv.getResult();
    }

    std::pair<bool, RegisterColoring> tryColor(lir::Function *f,
                                               Interference &interference,
                                               const SpillCosts &sc,
                                               lir::OperandManager *om) {
        int num_regs = 16;

        //
        // "Pruning" (degree < R)
        //
        Interference remaining = interference;
        std::vector<lir::Register *> pruned_regs;
        auto move_to_stack = [&](lir::Register *reg) {
            pruned_regs.push_back(reg);
            for (auto neighbor : remaining.at(reg))
                remaining[neighbor].erase(reg);
            remaining.erase(reg);
        };

        bool changed = true;
        while (changed) {
            changed = false;

            for (auto &[reg, edges] : remaining) {
                if (edges.size() >= num_regs)
                    continue;

                move_to_stack(reg);
                changed = true;
                break;
            }

            if (!remaining.empty()) {
                uint64_t min_weight = -1;
                auto min_reg = remaining.begin()->first;
                for (auto &[reg, edges] : remaining) {
                    auto cost = reg->getRegNum() == lir::RegisterNum::VIRTUAL
                                    ? sc.at(reg)
                                    : 100000;

                    auto cur_val = edges.size() == 0 ? 0 : cost / edges.size();
                    if (cur_val >= min_weight)
                        continue;

                    min_weight = cur_val;
                    min_reg = reg;
                }

                move_to_stack(min_reg);
                changed = true;
            }
        }

        //
        // Coloring Registers
        //
        auto coloring = getPrecoloring(f, om);
        std::unordered_set<lir::RegisterNum> seen_colors;
        for (auto &[_, color] : coloring)
            seen_colors.insert(color);

        std::vector<lir::RegisterNum> available_colors(seen_colors.begin(),
                                                       seen_colors.end());
        bool can_color = true;

        while (!pruned_regs.empty()) {
            auto reg = pruned_regs.back();
            pruned_regs.pop_back();
            if (coloring.contains(reg))
                continue;

            std::unordered_set<lir::RegisterNum> neighbor_colors;
            for (auto neighbor : interference[reg]) {
                auto iter = coloring.find(neighbor);
                if (iter == coloring.end())
                    continue;
                neighbor_colors.insert(iter->second);
            }

            for (auto color : available_colors) {
                if (neighbor_colors.contains(color))
                    continue;
                coloring[reg] = color;
                break;
            }

            if (!coloring.contains(reg)) {
                auto virtual_reg = dynamic_cast<lir::VirtualRegister *>(reg);
                if (virtual_reg) {
                    coloring[reg] = lir::RegisterNum::VIRTUAL;
                    can_color = false;
                    continue;
                }

                auto new_color =
                    lir::toSized(reg->getRegNum(), lir::DataSize::QUADWORD);
                coloring[reg] = new_color;
                available_colors.push_back(new_color);
            }
        }

        return {can_color, coloring};
    }

    void assignRegisters(lir::Function *f, RegisterColoring &coloring,
                         lir::OperandManager *om) {
        AssignmentVisitor cv(coloring, om);
        for (auto &i : f->getInstructions())
            i->accept(&cv);
    }

    void printColoring(RegisterColoring &coloring) {
        for (auto &[reg, color] : coloring)
            std::cout << reg->toString() << ": " << lir::toString(color)
                      << std::endl;
    }
} // namespace backend
