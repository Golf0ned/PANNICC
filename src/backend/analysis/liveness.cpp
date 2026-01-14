#include <iostream>

#include "backend/analysis/liveness.h"
#include "backend/lir/operand.h"

namespace backend {
    GenSetVisitor::GenSetVisitor(lir::OperandManager *om) : om(om) {}

    std::unordered_set<lir::Register *> GenSetVisitor::getResult() {
        return gen;
    }

    void GenSetVisitor::checkOperand(lir::Operand *o) {
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

    SuccessorVisitor::SuccessorVisitor(
        std::list<std::unique_ptr<lir::Instruction>> &instructions) {
        auto i = 0;
        for (auto instruction = instructions.begin();
             instruction != instructions.end(); instruction++) {
            auto label = dynamic_cast<lir::Label *>(instruction->get());
            if (label)
                label_index[label->getName()] = i;

            next_index[instruction->get()] =
                ++i == instructions.size() ? -1 : i;
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

    Liveness::Liveness(lir::Program &p)
        : program(p), gsv(GenSetVisitor(p.getOm())),
          ksv(KillSetVisitor(p.getOm())), sv(p.getInstructions()) {}

    void Liveness::computeLiveRanges() {
        auto size = program.getInstructions().size();
        gen.clear();
        kill.clear();
        std::vector<std::vector<int>> successors;

        for (auto &i : program.getInstructions()) {
            i->accept(&gsv);
            i->accept(&ksv);
            i->accept(&sv);

            gen.push_back(std::move(gsv.getResult()));
            kill.push_back(std::move(ksv.getResult()));
            successors.push_back(std::move(sv.getResult()));
        }

        in = std::vector<std::unordered_set<lir::Register *>>(size);
        out = std::vector<std::unordered_set<lir::Register *>>(size);

        std::unordered_set<lir::Register *> new_in, new_out;
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < size; i++) {
                new_in = gen[i];
                for (auto reg : out[i])
                    if (!kill[i].contains(reg))
                        new_in.insert(reg);

                new_out.clear();
                for (auto succ : successors[i])
                    new_out.insert(in[succ].begin(), in[succ].end());

                if (new_in != in[i]) {
                    changed = true;
                    in[i].swap(new_in);
                }

                if (new_out != out[i]) {
                    changed = true;
                    out[i].swap(new_out);
                }
            }
        }
    }

    void Liveness::printLiveness() {
        auto line = 0;
        for (auto &i : program.getInstructions()) {
            lir::ToStringVisitor tsv;
            i->accept(&tsv);
            std::cout << line << ": " << tsv.getResult() << std::endl;

            i->accept(&gsv);
            i->accept(&ksv);
            i->accept(&sv);

            auto gen = gsv.getResult();
            auto kill = ksv.getResult();
            auto successors = sv.getResult();

            std::cout << "- gen:        ";
            for (auto *reg : gen)
                std::cout << reg->toString() << ", ";
            std::cout << std::endl;

            std::cout << "- kill:       ";
            for (auto *reg : kill)
                std::cout << reg->toString() << ", ";
            std::cout << std::endl;

            std::cout << "- successors: ";
            for (auto i : successors)
                std::cout << i << ", ";
            std::cout << std::endl;

            std::cout << "- in:         ";
            for (auto *reg : in[line])
                std::cout << reg->toString() << ", ";
            std::cout << std::endl;

            std::cout << "- out:        ";
            for (auto *reg : out[line])
                std::cout << reg->toString() << ", ";
            std::cout << std::endl;

            line++;
        }
    }

    // TODO: sort the result vectors

    std::vector<std::vector<lir::Register *>> Liveness::getGen() {
        std::vector<std::vector<lir::Register *>> gen_list;

        for (auto &gen_set : gen) {
            gen_list.push_back(
                std::vector<lir::Register *>(gen_set.begin(), gen_set.end()));
        }

        return gen_list;
    }

    std::vector<std::vector<lir::Register *>> Liveness::getKill() {
        std::vector<std::vector<lir::Register *>> kill_list;

        for (auto &kill_set : kill) {
            kill_list.push_back(
                std::vector<lir::Register *>(kill_set.begin(), kill_set.end()));
        }

        return kill_list;
    }

    std::vector<std::vector<lir::Register *>> Liveness::getIn() {
        std::vector<std::vector<lir::Register *>> in_list;

        for (auto &in_set : in) {
            in_list.push_back(
                std::vector<lir::Register *>(in_set.begin(), in_set.end()));
        }

        return in_list;
    }

    std::vector<std::vector<lir::Register *>> Liveness::getOut() {
        std::vector<std::vector<lir::Register *>> out_list;

        for (auto &out_set : out) {
            out_list.push_back(
                std::vector<lir::Register *>(out_set.begin(), out_set.end()));
        }

        return out_list;
    }
} // namespace backend
