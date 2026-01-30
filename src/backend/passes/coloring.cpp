#include <iostream>

#include "backend/lir/operand.h"
#include "backend/passes/coloring.h"

namespace backend {
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
        // TODO: make changes to lir
    }

    void printColoring(RegisterColoring &coloring) {
        for (auto &[reg, color] : coloring) {
            auto color_str = color ? color->toString() : "UNCOLORABLE";
            std::cout << reg->toString() << ": " << color_str << std::endl;
        }
    }

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
} // namespace backend
