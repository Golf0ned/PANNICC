#include "backend/passes/coloring.h"
#include "backend/lir/operand.h"

namespace backend {
    std::pair<bool, RegisterColoring> tryColor(lir::Program &lir,
                                               Interference &interference) {
        int num_regs = 16;

        //
        // "Pruning"
        //
        Interference remaining = interference;
        std::vector<lir::Register *> pruned_regs;
        bool changed = true;
        while (changed) {
            changed = false;

            for (auto &[reg, edges] : remaining) {
                if (edges.size() >= num_regs)
                    continue;

                pruned_regs.push_back(reg);

                for (auto neighbor : edges)
                    remaining[neighbor].erase(reg);
                remaining.erase(reg);

                changed = true;
                break;
            }

            if (!remaining.empty()) {
                // TODO: find node with min spill cost divided by degree, push
                // onto stack

                // For now, we just stick topmost node on the stack lol
                pruned_regs.push_back(remaining.begin()->first);

                changed = true;
            }
        }

        //
        // Assigning Registers
        //
        RegisterColoring coloring;
        std::vector<lir::Register *> available_colors;
        bool can_color = true;

        // TODO: precolor constrained (shift.)
        // - walk program with visitor, get all constrained regs
        // - color physical + virtual reg the same "color" (reg) in coloring
        // - add physical to available_colors

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
} // namespace backend
