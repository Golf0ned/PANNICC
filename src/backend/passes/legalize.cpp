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
            auto &in = liveness[2], &out = liveness[3];
            auto registers_used = getUsedRegisters(liveness);
            auto function_stack_bytes = f->getStackBytes();

            auto &instructions = f->getInstructions();
            auto ret_iter = std::prev(instructions.end());

            auto rsp = om->getRegister(lir::RegisterNum::RSP);

            //
            // Preserve caller-saved registers, manage function args, and
            // allocate space around calls
            //
            auto has_call = false;
            size_t i_index = 0;
            for (auto i_iter = instructions.begin();
                 i_iter != instructions.end(); i_iter++) {
                auto call = dynamic_cast<lir::InstructionCall *>(i_iter->get());
                if (!call) {
                    i_index++;
                    continue;
                }

                has_call = true;

                auto next_iter = i_iter;

                // TODO: do we need to save from in at all? we currently save it
                // for potential arg collision issues
                auto &in_i = in[i_index];
                auto &out_i = out[i_index];
                std::vector<lir::Register *> save;
                std::unordered_set<lir::Register *> restore;
                for (auto reg_num : lir::getCallerSavedRegisters()) {
                    // TODO: use 64 bit once liveness is rewritten
                    auto sized_reg_num =
                        lir::toSized(reg_num, lir::DataSize::DOUBLEWORD);
                    if (sized_reg_num == lir::RegisterNum::EAX)
                        continue;

                    auto reg = om->getRegister(sized_reg_num);
                    if (in_i.contains(reg)) {
                        save.push_back(reg);
                    }
                    if (out_i.contains(reg))
                        restore.insert(reg);
                }

                // Allocate/deallocate stack space
                auto num_params = call->getArgs().size();
                auto call_stack_bytes =
                    (save.size() + (num_params > 6 ? num_params - 6 : 0)) * 8;
                if (call_stack_bytes) {
                    call_stack_bytes = (call_stack_bytes + 15) & -16;

                    auto stack_bytes = om->getImmediate(call_stack_bytes);

                    auto allocate = std::make_unique<lir::InstructionBinaryOp>(
                        lir::BinaryOp::SUB, reg_size, stack_bytes, rsp);
                    instructions.insert(i_iter, std::move(allocate));

                    auto deallocate =
                        std::make_unique<lir::InstructionBinaryOp>(
                            lir::BinaryOp::ADD, reg_size, stack_bytes, rsp);
                    next_iter = instructions.insert(std::next(i_iter),
                                                    std::move(deallocate));
                }

                // Save/restore registers
                std::unordered_map<lir::Register *, lir::Address *>
                    saved_values;
                for (auto reg : save) {
                    call_stack_bytes -= 8;

                    auto stack_slot =
                        om->getAddress(rsp, nullptr, nullptr,
                                       om->getImmediate(call_stack_bytes));
                    auto push = std::make_unique<lir::InstructionMov>(
                        lir::Extend::NONE, reg_size, reg_size, reg, stack_slot);
                    instructions.insert(i_iter, std::move(push));

                    saved_values[reg] = stack_slot;

                    if (!restore.contains(reg))
                        continue;

                    auto pop = std::make_unique<lir::InstructionMov>(
                        lir::Extend::NONE, reg_size, reg_size, stack_slot, reg);
                    next_iter =
                        instructions.insert(std::next(i_iter), std::move(pop));
                }

                // Store args
                auto &arg_regs = lir::getArgRegisters();
                auto &arg_vals = call->getArgs();
                for (size_t idx = 0; idx < num_params; idx++) {
                    auto param =
                        idx < 6 ? om->getRegister(arg_regs[idx])
                                : static_cast<lir::Operand *>(om->getAddress(
                                      rsp, nullptr, nullptr,
                                      om->getImmediate((idx - 6) * 8)));

                    auto val = arg_vals[idx];
                    auto val_reg = dynamic_cast<lir::Register *>(val);
                    if (val_reg && saved_values.contains(val_reg))
                        val = saved_values[val_reg];

                    auto mov = std::make_unique<lir::InstructionMov>(
                        lir::Extend::NONE, reg_size, reg_size, val, param);
                    instructions.insert(i_iter, std::move(mov));
                }

                i_iter = next_iter;
                i_index++;
            }

            //
            // Preserve callee-saved registers
            //
            auto &callee_saved_vector = lir::getCalleeSavedRegisters();
            auto callee_saved = std::unordered_set(callee_saved_vector.begin(),
                                                   callee_saved_vector.end());

            for (auto reg_num : registers_used) {
                if (!callee_saved.contains(reg_num) ||
                    reg_num == lir::RegisterNum::RSP)
                    continue;

                auto reg_to_save = om->getRegister(reg_num);
                auto stack_slot =
                    om->getAddress(rsp, nullptr, nullptr,
                                   om->getImmediate(function_stack_bytes));

                auto push = std::make_unique<lir::InstructionMov>(
                    lir::Extend::NONE, reg_size, reg_size, reg_to_save,
                    stack_slot);
                instructions.push_front(std::move(push));

                auto pop = std::make_unique<lir::InstructionMov>(
                    lir::Extend::NONE, reg_size, reg_size, stack_slot,
                    reg_to_save);
                instructions.insert(ret_iter, std::move(pop));

                function_stack_bytes += 8;
            }

            //
            // Pad bytes to 16 byte alignment if containing call
            //
            if (has_call)
                function_stack_bytes = (function_stack_bytes + 15) & -16;

            //
            // Replace stack arg operands
            //
            ReplaceStackArgVisitor rsav(function_stack_bytes, om);
            for (auto &i : instructions)
                i->accept(&rsav);

            //
            // Manage stack space
            //
            if (!function_stack_bytes)
                continue;

            auto stack_bytes = om->getImmediate(function_stack_bytes);

            auto allocate = std::make_unique<lir::InstructionBinaryOp>(
                lir::BinaryOp::SUB, reg_size, stack_bytes, rsp);
            instructions.push_front(std::move(allocate));

            auto deallocate = std::make_unique<lir::InstructionBinaryOp>(
                lir::BinaryOp::ADD, reg_size, stack_bytes, rsp);
            instructions.insert(ret_iter, std::move(deallocate));
        }
    }
} // namespace backend
;
