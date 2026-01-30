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

        auto color_size = lir::DataSize::QUADWORD;
        auto color = lir::toSized(constrained->getConstraint(), color_size);

        auto physical_reg = om->getRegister(color);
        auto virtual_reg = om->getRegister(constrained->getName());

        precolorings[physical_reg] = physical_reg;
        precolorings[virtual_reg] = physical_reg;
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

    ColoringVisitor::ColoringVisitor(const RegisterColoring &coloring,
                                     lir::OperandManager *om)
        : coloring(coloring), om(om) {}

    lir::Operand *ColoringVisitor::tryColorOperand(lir::Operand *operand) {
        auto constrained = dynamic_cast<lir::ConstrainedRegister *>(operand);
        if (constrained)
            return coloring.at(om->getRegister(constrained->getName()));

        // Skip physical for now because I'm too lazy to deal with sized
        auto reg = dynamic_cast<lir::VirtualRegister *>(operand);
        if (reg)
            return coloring.at(reg);

        auto address = dynamic_cast<lir::Address *>(operand);
        if (address) {
            auto base = address->getBase(), index = address->getIndex();
            auto new_base = base ? coloring.at(base) : nullptr;
            auto new_index = index ? coloring.at(index) : nullptr;

            return om->getAddress(new_base, new_index, address->getScale(),
                                  address->getDisplacement());
        }

        return nullptr;
    }

    void ColoringVisitor::visit(lir::Instruction *i) {}

    void ColoringVisitor::visit(lir::Label *l) {}

    void ColoringVisitor::visit(lir::InstructionMov *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(src);

        auto dst = tryColorOperand(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void ColoringVisitor::visit(lir::InstructionPush *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(src);
    }

    void ColoringVisitor::visit(lir::InstructionPop *i) {
        auto dst = tryColorOperand(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void ColoringVisitor::visit(lir::InstructionConvert *i) {}

    void ColoringVisitor::visit(lir::InstructionBinaryOp *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(src);

        auto dst = tryColorOperand(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void ColoringVisitor::visit(lir::InstructionSpecialOp *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(src);
    }

    void ColoringVisitor::visit(lir::InstructionLea *i) {
        auto src = tryColorOperand(i->getSrc());
        if (src)
            i->setSrc(static_cast<lir::Address *>(src));

        auto dst = tryColorOperand(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void ColoringVisitor::visit(lir::InstructionCmp *i) {
        auto src1 = tryColorOperand(i->getSrc1());
        if (src1)
            i->setSrc1(src1);

        auto src2 = tryColorOperand(i->getSrc2());
        if (src2)
            i->setSrc2(src2);
    }

    void ColoringVisitor::visit(lir::InstructionJmp *i) {}

    void ColoringVisitor::visit(lir::InstructionCJmp *i) {}

    void ColoringVisitor::visit(lir::InstructionCall *i) {}

    void ColoringVisitor::visit(lir::InstructionRet *i) {}

    void ColoringVisitor::visit(lir::InstructionVirtualCall *i) {
        auto args = i->getArgs();

        for (auto &arg : args) {
            auto colored = tryColorOperand(arg);
            if (colored)
                arg = colored;
        }

        i->setArgs(args);
    }

    void ColoringVisitor::visit(lir::InstructionUnknown *i) {}

    RegisterColoring getPrecoloring(lir::Program &lir) {
        PrecoloringVisitor pcv(lir.getOm());
        for (auto &f : lir.getFunctions())
            for (auto &i : f->getInstructions())
                i->accept(&pcv);

        return pcv.getResult();
    }

    std::pair<bool, RegisterColoring> tryColor(lir::Program &lir,
                                               Interference &interference) {
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
                // TODO: find reg with min spill cost divided by degree,
                // and push onto stack
                // We haven't implemented spill cost yet!
                // Therefore, big number if physical reg
                uint64_t min_val = -1;
                auto min_reg = remaining.begin()->first;
                for (auto &[reg, edges] : remaining) {
                    auto cost = reg->getRegNum() == lir::RegisterNum::VIRTUAL
                                    ? 1000
                                    : 100000;

                    auto cur_val = edges.size() == 0 ? 0 : cost / edges.size();
                    if (cur_val >= min_val)
                        continue;

                    min_val = cur_val;
                    min_reg = reg;
                }

                move_to_stack(min_reg);
                changed = true;
            }
        }

        //
        // Assigning Registers
        //
        auto coloring = getPrecoloring(lir);
        std::unordered_set<lir::Register *> seen_colors;
        for (auto &[_, color] : coloring)
            seen_colors.insert(color);

        std::vector<lir::Register *> available_colors(seen_colors.begin(),
                                                      seen_colors.end());
        bool can_color = true;

        while (!pruned_regs.empty()) {
            auto reg = pruned_regs.back();
            pruned_regs.pop_back();
            if (coloring.contains(reg))
                continue;

            std::unordered_set<lir::Register *> neighbor_colors;
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
                    coloring[reg] = nullptr;
                    can_color = false;
                    continue;
                }

                coloring[reg] = reg;
                available_colors.push_back(reg);
            }
        }

        return {can_color, coloring};
    }

    void colorRegisters(lir::Program &lir, RegisterColoring &coloring) {
        ColoringVisitor cv(coloring, lir.getOm());
        for (auto &f : lir.getFunctions())
            for (auto &i : f->getInstructions())
                i->accept(&cv);
    }

    void printColoring(RegisterColoring &coloring) {
        for (auto &[reg, color] : coloring) {
            auto color_str = color ? color->toString() : "UNCOLORABLE";
            std::cout << reg->toString() << ": " << color_str << std::endl;
        }
    }
} // namespace backend
