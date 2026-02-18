#include <algorithm>

#include "backend/lir/register_num.h"
#include "backend/passes/legalize.h"
#include "backend/passes/liveness.h"

namespace backend {
    // TODO: unhardcode
    constexpr auto reg_size = lir::DataSize::QUADWORD;

    std::vector<lir::RegisterNum> getUsedRegisters(Liveness &l) {
        auto &gen = l[0], &kill = l[1];

        // TODO: do this in a less awful way
        std::unordered_set<lir::RegisterNum> registers_used;

        for (size_t i = 0; i < gen.size(); i++) {
            auto &gen_i = gen[i], &kill_i = kill[i];
            for (auto reg : gen_i) {
                auto sized_reg_num = lir::toSized(reg->getRegNum(), reg_size);
                registers_used.insert(sized_reg_num);
            }
            for (auto reg : kill_i) {
                auto sized_reg_num = lir::toSized(reg->getRegNum(), reg_size);
                registers_used.insert(sized_reg_num);
            }
        }

        std::vector<lir::RegisterNum> res(registers_used.begin(),
                                          registers_used.end());
        std::sort(res.begin(), res.end());
        return res;
    }

    void legalize(lir::Program &lir) {
        auto om = lir.getOm();

        for (auto &f : lir.getFunctions()) {
            auto liveness = computeLiveness(f.get(), om);
            auto registers_used = getUsedRegisters(liveness);
            auto total_stack_bytes = f->getStackBytes();

            auto &instructions = f->getInstructions();
            auto ret_iter = std::prev(instructions.end());

            auto rsp = om->getRegister(lir::RegisterNum::RSP);

            // TODO: save/restore caller-saved registers around function calls

            //
            // Save callee-preserved registers
            //
            auto callee_saved_vector = lir::getCalleeSavedRegisters();
            auto callee_saved = std::unordered_set(callee_saved_vector.begin(),
                                                   callee_saved_vector.end());

            for (auto reg_num : registers_used) {
                if (!callee_saved.contains(reg_num) ||
                    reg_num == lir::RegisterNum::RSP)
                    continue;

                auto reg_to_save = om->getRegister(reg_num);
                auto stack_slot = om->getAddress(
                    rsp, nullptr, nullptr, om->getImmediate(total_stack_bytes));

                auto push = std::make_unique<lir::InstructionMov>(
                    lir::Extend::NONE, reg_size, reg_size, reg_to_save,
                    stack_slot);
                instructions.push_front(std::move(push));

                auto pop = std::make_unique<lir::InstructionMov>(
                    lir::Extend::NONE, reg_size, reg_size, stack_slot,
                    reg_to_save);
                instructions.insert(ret_iter, std::move(pop));

                total_stack_bytes += 8;
            }

            //
            // Replace stack arg operands
            // TODO
            //
            // TODO: pad total bytes to 16 byte alignment if containing call

            //
            // Manage stack space
            //
            if (!total_stack_bytes)
                return;

            auto stack_bytes = om->getImmediate(total_stack_bytes);

            auto allocate = std::make_unique<lir::InstructionBinaryOp>(
                lir::BinaryOp::SUB, reg_size, stack_bytes, rsp);
            instructions.push_front(std::move(allocate));

            auto deallocate = std::make_unique<lir::InstructionBinaryOp>(
                lir::BinaryOp::ADD, reg_size, stack_bytes, rsp);
            instructions.insert(ret_iter, std::move(deallocate));
        }
    }
} // namespace backend
