#include "middleend/transform/inst_combine.h"
#include "middleend/utils/replace_uses.h"

namespace middleend {
    void InstCombine::run(mir::Program &p) {
        // for (auto &f : p.getFunctions()) {
        //     bool changed = true;
        //     while (changed) {
        //         changed = false;
        //         std::unordered_map<mir::BasicBlock *, std::vector<size_t>>
        //             to_erase;
        //
        //         for (auto &bb : f.getBasicBlocks()) {
        //             auto &instructions = bb->getInstructions();
        //             for (size_t ind = 0; ind < instructions.size(); ind++) {
        //                 auto i = instructions[ind].get();
        //
        //                 auto bin_op =
        //                     dynamic_cast<mir::InstructionBinaryOp *>(i);
        //                 if (!bin_op)
        //                     continue;
        //
        //                 auto left =
        //                     dynamic_cast<mir::Literal *>(bin_op->getLeft());
        //                 if (!left)
        //                     continue;
        //
        //                 auto right =
        //                     dynamic_cast<mir::Literal *>(bin_op->getLeft());
        //                 if (!right)
        //                     continue;
        //
        //                 uint64_t folded_val;
        //                 switch (bin_op->getOp()) {
        //                 // TODO: worry about overflow
        //                 case mir::BinaryOp::ADD:
        //                     folded_val = left->getValue() +
        //                     right->getValue(); break;
        //                 case mir::BinaryOp::SUB:
        //                     folded_val = left->getValue() -
        //                     right->getValue(); break;
        //                 case mir::BinaryOp::MUL:
        //                     folded_val = left->getValue() *
        //                     right->getValue(); break;
        //                 case mir::BinaryOp::AND:
        //                     folded_val = left->getValue() &
        //                     right->getValue(); break;
        //                 }
        //
        //                 auto type = mir::Type::I32;
        //                 auto folded_literal =
        //                     std::make_unique<mir::Literal>(type, folded_val);
        //
        //                 ReplaceUsesVisitor visitor(bin_op,
        //                 folded_literal.get(),
        //                                            bb.get());
        //                 for (auto [use, _] : bin_op->getUses()) {
        //                     use->accept(&visitor);
        //                 }
        //
        //                 to_erase[bb.get()].push_back(ind);
        //             }
        //         }
        //
        //         for (auto &bb : f.getBasicBlocks()) {
        //             auto &instructions = bb->getInstructions();
        //             auto &block_erase = to_erase[bb.get()];
        //
        //             std::vector<std::unique_ptr<mir::Instruction>> new_body;
        //             new_body.reserve(instructions.size() -
        //             block_erase.size());
        //
        //             size_t to_erase_ind = 0;
        //             for (size_t ind = 0; ind < instructions.size(); ind++) {
        //                 if (to_erase_ind < block_erase.size() &&
        //                     ind == block_erase[to_erase_ind]) {
        //                     to_erase_ind++;
        //                     continue;
        //                 }
        //                 new_body.push_back(std::move(instructions[ind]));
        //             }
        //
        //             bb->getInstructions().swap(new_body);
        //         }
        //     }
        // }
    }
} // namespace middleend
