#include <ranges>

#include "middleend/transform/inst_combine.h"
#include "middleend/utils/erase_uses.h"
#include "middleend/utils/replace_uses.h"

namespace middleend {
    void InstCombine::run(mir::Program &p) {
        EraseUsesVisitor erase;
        for (auto &f : p.getFunctions()) {
            bool changed = true;
            while (changed) {
                changed = false;
                std::vector<std::unique_ptr<mir::Instruction>> to_drop;
                for (auto &bb : f->getBasicBlocks()) {
                    auto &instructions = bb->getInstructions();
                    auto iter = instructions.begin();
                    while (iter != instructions.end()) {
                        auto i = iter->get();

                        auto tryConstantFold = [&]() {
                            auto bin_op =
                                dynamic_cast<mir::InstructionBinaryOp *>(i);
                            if (!bin_op)
                                return false;

                            auto left =
                                dynamic_cast<mir::Literal *>(bin_op->getLeft());
                            if (!left)
                                return false;

                            auto right =
                                dynamic_cast<mir::Literal *>(bin_op->getLeft());
                            if (!right)
                                return false;

                            uint32_t lhs = static_cast<uint32_t>(
                                static_cast<int32_t>(left->getValue()));
                            uint32_t rhs = static_cast<uint32_t>(
                                static_cast<int32_t>(right->getValue()));

                            int32_t folded_val;
                            switch (bin_op->getOp()) {
                            // TODO: worry about overflow
                            // TODO: worry about types
                            case mir::BinaryOp::ADD:
                                folded_val = lhs + rhs;
                                break;
                            case mir::BinaryOp::SUB:
                                folded_val = lhs - rhs;
                                break;
                            case mir::BinaryOp::MUL:
                                folded_val = lhs * rhs;
                                break;
                            case mir::BinaryOp::AND:
                                folded_val = lhs & rhs;
                                break;
                            }

                            auto type = mir::Type::I32;
                            auto extended_val = static_cast<uint64_t>(
                                static_cast<int64_t>(folded_val));
                            auto folded_literal =
                                p.getLiteral(type, extended_val);

                            ReplaceUsesVisitor visitor(bin_op, folded_literal);
                            auto uses_range =
                                std::views::keys(bin_op->getUses());
                            std::vector<mir::Instruction *> uses(
                                uses_range.begin(), uses_range.end());
                            for (auto &use : uses)
                                use->accept(&visitor);

                            i->accept(&erase);
                            to_drop.push_back(std::move(*iter));
                            iter = instructions.erase(iter);
                            return true;
                        };

                        // clang-format off
                        if (false
                            || tryConstantFold()
                        ) {
                            // clang-format on
                            changed = true;
                            continue;
                        }

                        iter++;
                    }
                }
            }
        }
    }
} // namespace middleend
