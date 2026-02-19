#include <algorithm>

#include "backend/lir/register_num.h"
#include "backend/passes/legalize.h"
#include "backend/passes/liveness.h"

namespace backend {
    // TODO: unhardcode
    constexpr auto reg_size = lir::DataSize::QUADWORD;

    ReplaceStackArgVisitor::ReplaceStackArgVisitor(uint64_t stack_bytes,
                                                   lir::OperandManager *om)
        : stack_bytes(stack_bytes), om(om) {}

    lir::Operand *ReplaceStackArgVisitor::tryReplace(lir::Operand *operand) {
        auto stack_arg = dynamic_cast<lir::StackArg *>(operand);
        if (!stack_arg)
            return nullptr;

        auto stack_addr = stack_bytes + stack_arg->getArgNum() * 8;
        auto displacement = om->getImmediate(stack_addr);
        auto rsp = om->getRegister(lir::RegisterNum::RSP);
        return om->getAddress(rsp, nullptr, nullptr, displacement);
    }

    void ReplaceStackArgVisitor::visit(lir::Instruction *i) {}

    void ReplaceStackArgVisitor::visit(lir::Label *l) {}

    void ReplaceStackArgVisitor::visit(lir::InstructionMov *i) {
        auto src = tryReplace(i->getSrc());
        if (src)
            i->setSrc(src);

        auto dst = tryReplace(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void ReplaceStackArgVisitor::visit(lir::InstructionPush *i) {
        auto src = tryReplace(i->getSrc());
        if (src)
            i->setSrc(src);
    }

    void ReplaceStackArgVisitor::visit(lir::InstructionPop *i) {
        auto dst = tryReplace(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void ReplaceStackArgVisitor::visit(lir::InstructionConvert *i) {}

    void ReplaceStackArgVisitor::visit(lir::InstructionBinaryOp *i) {
        auto src = tryReplace(i->getSrc());
        if (src)
            i->setSrc(src);

        auto dst = tryReplace(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void ReplaceStackArgVisitor::visit(lir::InstructionSpecialOp *i) {
        auto src = tryReplace(i->getSrc());
        if (src)
            i->setSrc(src);
    }

    void ReplaceStackArgVisitor::visit(lir::InstructionLea *i) {
        auto src = tryReplace(i->getSrc());
        if (src)
            i->setSrc(static_cast<lir::Address *>(src));

        auto dst = tryReplace(i->getDst());
        if (dst)
            i->setDst(dst);
    }

    void ReplaceStackArgVisitor::visit(lir::InstructionCmp *i) {
        auto src1 = tryReplace(i->getSrc1());
        if (src1)
            i->setSrc1(src1);

        auto src2 = tryReplace(i->getSrc2());
        if (src2)
            i->setSrc2(src2);
    }

    void ReplaceStackArgVisitor::visit(lir::InstructionJmp *i) {}

    void ReplaceStackArgVisitor::visit(lir::InstructionCJmp *i) {}

    void ReplaceStackArgVisitor::visit(lir::InstructionCall *i) {
        auto args = i->getArgs();

        for (auto &arg : args) {
            auto stack_arg = tryReplace(arg);
            if (stack_arg)
                arg = stack_arg;
        }

        i->setArgs(args);
    }

    void ReplaceStackArgVisitor::visit(lir::InstructionRet *i) {}

    void ReplaceStackArgVisitor::visit(lir::InstructionUnknown *i) {}

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

            // TODO: generate calling convention for calls
            // - allocate space for caller-saved registers, params 7+,
            //   and 16 byte padding
            // - save caller-saved registers
            // - store params in correct registers
            //   (use mapping from caller-saved registers to stack space)
            // - [call]
            // - restore caller-saved registers
            // - deallocate space
            // TODO: track if function has call
            auto has_call = true;

            //
            // Save callee-saved registers
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
            // Pad bytes to 16 byte alignment if containing call
            //
            if (has_call && total_stack_bytes % 16 != 0)
                total_stack_bytes = (total_stack_bytes + 15) & -16;

            //
            // Replace stack arg operands
            //
            ReplaceStackArgVisitor rsav(total_stack_bytes, om);
            for (auto &i : instructions)
                i->accept(&rsav);

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
