#include "backend/passes/legalize.h"
#include "backend/lir/register_num.h"
#include "backend/passes/liveness.h"

namespace backend {
    void legalize(lir::Program &lir) {
        auto om = lir.getOm();

        for (auto &f : lir.getFunctions()) {
            // TODO: unhardcode
            constexpr auto reg_size = lir::DataSize::QUADWORD;

            auto liveness = computeLiveness(f.get(), om);
            auto &gen = liveness[0], &kill = liveness[1];

            // TODO: do this in a less awful way
            std::unordered_set<lir::RegisterNum> registers_used;
            for (size_t i = 0; i < gen.size(); i++) {
                auto &gen_i = gen[i], &kill_i = kill[i];
                for (auto reg : gen_i) {
                    auto sized_reg_num =
                        lir::toSized(reg->getRegNum(), reg_size);
                    registers_used.insert(sized_reg_num);
                }
                for (auto reg : kill_i) {
                    auto sized_reg_num =
                        lir::toSized(reg->getRegNum(), reg_size);
                    registers_used.insert(sized_reg_num);
                }
            }

            auto &instructions = f->getInstructions();
            auto ret_iter = std::prev(instructions.end());

            // TODO: replace stack args
            // TODO: save/restore caller-saved registers around function calls

            // TODO: allocate/deallocate stack space

            for (auto reg_num : lir::getCalleeSavedRegisters()) {
                if (!registers_used.contains(reg_num) ||
                    reg_num == lir::RegisterNum::RSP)
                    continue;

                auto operand = om->getRegister(reg_num);

                auto push =
                    std::make_unique<lir::InstructionPush>(reg_size, operand);
                instructions.push_front(std::move(push));

                auto pop =
                    std::make_unique<lir::InstructionPop>(reg_size, operand);
                instructions.insert(ret_iter, std::move(pop));
            }
        }
    }
} // namespace backend
