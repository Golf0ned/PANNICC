#include <iostream>

#include "backend/lir/lir.h"
#include "backend/lir/operand.h"
#include "backend/passes/liveness.h"

namespace backend {
    GenSetVisitor::GenSetVisitor(lir::OperandManager *om) : om(om) {}

    std::unordered_set<lir::Register *> GenSetVisitor::getResult() {
        return gen;
    }

    void GenSetVisitor::checkOperand(lir::Operand *o) {
        auto constrained = dynamic_cast<lir::ConstrainedRegister *>(o);
        if (constrained) {
            auto inner_reg =
                om->getRegister(constrained->getName(), constrained->getSize());
            gen.insert(inner_reg);
            return;
        }

        auto reg = dynamic_cast<lir::Register *>(o);
        if (reg) {
            gen.insert(reg);
            return;
        }

        auto address = dynamic_cast<lir::Address *>(o);
        if (address) {
            auto base = address->getBase();
            if (base)
                gen.insert(base);

            auto index = address->getIndex();
            if (index)
                gen.insert(index);
        }
    }

    void GenSetVisitor::addRegister(lir::RegisterNum reg) {
        auto physical_reg = om->getRegister(reg);
        gen.insert(physical_reg);
    }

    void GenSetVisitor::visit(lir::Instruction *i) {}

    void GenSetVisitor::visit(lir::Label *l) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionMov *i) {
        gen.clear();
        checkOperand(i->getSrc());
    }

    void GenSetVisitor::visit(lir::InstructionPush *i) {
        gen.clear();
        addRegister(lir::RegisterNum::RSP);
        checkOperand(i->getSrc());
    }

    void GenSetVisitor::visit(lir::InstructionPop *i) {
        gen.clear();
        addRegister(lir::RegisterNum::RSP);
    }

    void GenSetVisitor::visit(lir::InstructionConvert *i) {
        gen.clear();
        addRegister(lir::toSized(lir::RegisterNum::RAX, i->getFrom()));
    }

    void GenSetVisitor::visit(lir::InstructionBinaryOp *i) {
        gen.clear();
        checkOperand(i->getSrc());
        checkOperand(i->getDst());
    }

    void GenSetVisitor::visit(lir::InstructionSpecialOp *i) {
        gen.clear();
        checkOperand(i->getSrc());
        if (i->getOp() == lir::BinaryOp::IDIV)
            addRegister(lir::RegisterNum::EAX);
    }

    void GenSetVisitor::visit(lir::InstructionLea *i) {
        gen.clear();
        checkOperand(i->getSrc());
    }

    void GenSetVisitor::visit(lir::InstructionCmp *i) {
        gen.clear();
        checkOperand(i->getSrc1());
        checkOperand(i->getSrc2());
    }

    void GenSetVisitor::visit(lir::InstructionJmp *i) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionCJmp *i) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionCall *i) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionRet *i) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionVirtualCall *i) {
        gen.clear();
        for (auto arg : i->getArgs())
            checkOperand(arg);
    }

    void GenSetVisitor::visit(lir::InstructionUnknown *i) {}

    KillSetVisitor::KillSetVisitor(lir::OperandManager *om) : om(om) {}

    std::unordered_set<lir::Register *> KillSetVisitor::getResult() {
        return kill;
    }

    void KillSetVisitor::checkOperand(lir::Operand *o) {
        auto reg = dynamic_cast<lir::Register *>(o);
        if (reg) {
            kill.insert(reg);
            return;
        }
    }

    void KillSetVisitor::addRegister(lir::RegisterNum reg) {
        auto physical_reg = om->getRegister(reg);
        kill.insert(physical_reg);
    }

    void KillSetVisitor::visit(lir::Instruction *i) {}

    void KillSetVisitor::visit(lir::Label *l) { kill.clear(); }

    void KillSetVisitor::visit(lir::InstructionMov *i) {
        kill.clear();
        checkOperand(i->getDst());
    }

    void KillSetVisitor::visit(lir::InstructionPush *i) {
        kill.clear();
        addRegister(lir::RegisterNum::RSP);
    }

    void KillSetVisitor::visit(lir::InstructionPop *i) {
        kill.clear();
        addRegister(lir::RegisterNum::RSP);
        checkOperand(i->getDst());
    }

    void KillSetVisitor::visit(lir::InstructionConvert *i) {
        kill.clear();

        auto from = i->getFrom();
        auto to = i->getTo();

        addRegister(lir::toSized(lir::RegisterNum::RAX, to));
        if (from == to)
            addRegister(lir::toSized(lir::RegisterNum::RDX, to));
    }

    void KillSetVisitor::visit(lir::InstructionBinaryOp *i) {
        kill.clear();
        checkOperand(i->getDst());
    }

    void KillSetVisitor::visit(lir::InstructionSpecialOp *i) {
        kill.clear();

        if (i->getOp() == lir::BinaryOp::IDIV)
            addRegister(lir::toSized(lir::RegisterNum::RAX, i->getSize()));
    }

    void KillSetVisitor::visit(lir::InstructionLea *i) {
        kill.clear();
        checkOperand(i->getDst());
    }

    void KillSetVisitor::visit(lir::InstructionCmp *i) { kill.clear(); }

    void KillSetVisitor::visit(lir::InstructionJmp *i) { kill.clear(); }

    void KillSetVisitor::visit(lir::InstructionCJmp *i) { kill.clear(); }

    // TODO: is this correct?
    void KillSetVisitor::visit(lir::InstructionCall *i) { kill.clear(); }

    void KillSetVisitor::visit(lir::InstructionRet *i) { kill.clear(); }

    void KillSetVisitor::visit(lir::InstructionVirtualCall *i) {
        kill.clear();
        for (auto reg : lir::getCallerSavedRegisters())
            addRegister(reg);
    }

    void KillSetVisitor::visit(lir::InstructionUnknown *i) {}

    SuccessorVisitor::SuccessorVisitor(lir::Function *f) {
        auto idx = 0;
        auto &instructions = f->getInstructions();
        for (auto &i : instructions) {
            auto label = dynamic_cast<lir::Label *>(i.get());
            if (label)
                label_index[label->getName()] = idx;

            next_index[i.get()] = ++idx == instructions.size() ? -1 : idx;
        }
    }

    std::vector<int> SuccessorVisitor::getResult() { return successors; }

    void SuccessorVisitor::visit(lir::Instruction *i) {}

    void SuccessorVisitor::visit(lir::Label *l) {
        successors.clear();
        successors.push_back(next_index[l]);
    }

    void SuccessorVisitor::visit(lir::InstructionMov *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionPush *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionPop *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionConvert *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionBinaryOp *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionSpecialOp *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionLea *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionCmp *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionJmp *i) {
        successors.clear();
        successors.push_back(label_index[i->getLabel()]);
    }

    void SuccessorVisitor::visit(lir::InstructionCJmp *i) {
        successors.clear();
        successors.push_back(label_index[i->getLabel()]);
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionCall *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionRet *i) { successors.clear(); }

    void SuccessorVisitor::visit(lir::InstructionVirtualCall *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    void SuccessorVisitor::visit(lir::InstructionUnknown *i) {
        successors.clear();
        successors.push_back(next_index[i]);
    }

    Liveness computeLiveness(lir::Function *f, lir::OperandManager *om) {
        GenSetVisitor gsv(om);
        KillSetVisitor ksv(om);
        SuccessorVisitor sv(f);

        auto &instructions = f->getInstructions();

        std::vector<RegisterSet> gen, kill;
        std::vector<std::vector<int>> successors;
        for (auto &i : instructions) {
            i->accept(&gsv);
            i->accept(&ksv);
            i->accept(&sv);

            gen.push_back(std::move(gsv.getResult()));
            kill.push_back(std::move(ksv.getResult()));
            successors.push_back(std::move(sv.getResult()));
        }

        auto size = instructions.size();
        std::vector<RegisterSet> in(size), out(size);

        std::unordered_set<lir::Register *> cur_in, cur_out;
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < size; i++) {
                cur_in = gen[i];
                for (auto reg : out[i])
                    if (!kill[i].contains(reg))
                        cur_in.insert(reg);

                cur_out.clear();
                for (auto succ : successors[i])
                    cur_out.insert(in[succ].begin(), in[succ].end());

                if (cur_in != in[i]) {
                    changed = true;
                    in[i].swap(cur_in);
                }

                if (cur_out != out[i]) {
                    changed = true;
                    out[i].swap(cur_out);
                }
            }
        }

        return {gen, kill, in, out};
    }

    void printLiveness(lir::Function *f, Liveness &l) {
        std::array<std::string, 4> sets = {"gen", "kill", "in", "out"};

        std::cout << f->getName() << " {\n";
        size_t i_index = 0;
        for (auto &i : f->getInstructions()) {
            lir::ToStringVisitor tsv;
            i->accept(&tsv);
            std::cout << "    " << i_index << ": " << tsv.getResult() << '\n';

            for (size_t set_index = 0; set_index < 4; set_index++) {
                std::cout << "    - " << sets[set_index] << ": {";
                auto regs = l[set_index][i_index];
                for (auto reg = regs.begin(); reg != regs.end(); reg++) {
                    if (reg != regs.begin())
                        std::cout << ", ";
                    std::cout << (*reg)->toString();
                }
                std::cout << "}\n";
            }

            i_index++;
        }
        std::cout << "}" << std::endl;
    }
} // namespace backend
