#include <iostream>

#include "backend/analysis/liveness.h"

namespace backend {
    GenSetVisitor::GenSetVisitor(lir::OperandManager *om) : om(om) {}

    RegisterSet GenSetVisitor::getResult() { return gen; }

    void GenSetVisitor::visit(lir::Instruction *i) {}

    void GenSetVisitor::visit(lir::Label *l) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionMov *i) {
        gen.clear();

        auto src = dynamic_cast<lir::Register *>(i->getSrc());
        if (src)
            gen.insert(src);
    }

    void GenSetVisitor::visit(lir::InstructionPush *i) {
        gen.clear();

        auto src = dynamic_cast<lir::Register *>(i->getSrc());
        if (src)
            gen.insert(src);
    }

    void GenSetVisitor::visit(lir::InstructionPop *i) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionConvert *i) {
        gen.clear();

        auto from = lir::toSized(lir::RegisterNum::RAX, i->getFrom());
        auto reg = om->getRegister(from);
        gen.insert(reg);
    }

    void GenSetVisitor::visit(lir::InstructionBinaryOp *i) {
        gen.clear();

        auto src = dynamic_cast<lir::Register *>(i->getSrc());
        if (src)
            gen.insert(src);

        auto dst = dynamic_cast<lir::Register *>(i->getDst());
        if (dst)
            gen.insert(dst);
    }

    void GenSetVisitor::visit(lir::InstructionSpecialOp *i) {
        gen.clear();

        auto src = dynamic_cast<lir::Register *>(i->getSrc());
        if (src)
            gen.insert(src);

        if (i->getOp() == lir::BinaryOp::IDIV)
            gen.insert(om->getRegister(lir::RegisterNum::EAX));
    }

    void GenSetVisitor::visit(lir::InstructionLea *i) {
        gen.clear();

        auto address = i->getSrc();

        auto base = address->getBase();
        if (base)
            gen.insert(base);

        auto index = address->getIndex();
        if (index)
            gen.insert(index);
    }

    void GenSetVisitor::visit(lir::InstructionCmp *i) {
        gen.clear();

        auto src1 = dynamic_cast<lir::Register *>(i->getSrc1());
        if (src1)
            gen.insert(src1);

        auto src2 = dynamic_cast<lir::Register *>(i->getSrc2());
        if (src2)
            gen.insert(src2);
    }

    void GenSetVisitor::visit(lir::InstructionJmp *i) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionCJmp *i) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionCall *i) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionRet *i) { gen.clear(); }

    void GenSetVisitor::visit(lir::InstructionPhi *i) {
        gen.clear();

        for (auto src : i->getSrc()) {
            auto reg = dynamic_cast<lir::Register *>(src);
            if (reg)
                gen.insert(reg);
        }
    }

    void GenSetVisitor::visit(lir::InstructionVirtual *i) {
        i->getInstruction()->accept(this);
    }

    void GenSetVisitor::visit(lir::InstructionUnknown *i) {}

    KillSetVisitor::KillSetVisitor(lir::OperandManager *om) : om(om) {}

    RegisterSet KillSetVisitor::getResult() { return kill; }

    void KillSetVisitor::visit(lir::Instruction *i) {}

    void KillSetVisitor::visit(lir::Label *l) { kill.clear(); }

    void KillSetVisitor::visit(lir::InstructionMov *i) {
        kill.clear();

        auto dst = dynamic_cast<lir::Register *>(i->getDst());
        if (dst)
            kill.insert(dst);
    }

    void KillSetVisitor::visit(lir::InstructionPush *i) {}

    void KillSetVisitor::visit(lir::InstructionPop *i) {
        kill.clear();

        auto dst = dynamic_cast<lir::Register *>(i->getDst());
        if (dst)
            kill.insert(dst);
    }

    void KillSetVisitor::visit(lir::InstructionConvert *i) {
        kill.clear();

        auto from = i->getFrom();
        auto to = i->getTo();

        kill.insert(om->getRegister(lir::toSized(lir::RegisterNum::RAX, to)));

        if (from == to) {
            kill.insert(
                om->getRegister(lir::toSized(lir::RegisterNum::RDX, to)));
        }
    }

    void KillSetVisitor::visit(lir::InstructionBinaryOp *i) {
        kill.clear();

        auto dst = dynamic_cast<lir::Register *>(i->getDst());
        if (dst)
            kill.insert(dst);
    }

    void KillSetVisitor::visit(lir::InstructionSpecialOp *i) {
        kill.clear();

        if (i->getOp() == lir::BinaryOp::IDIV)
            kill.insert(om->getRegister(
                lir::toSized(lir::RegisterNum::RAX, i->getSize())));
    }

    void KillSetVisitor::visit(lir::InstructionLea *i) {
        kill.clear();

        auto dst = dynamic_cast<lir::Register *>(i->getDst());
        if (dst)
            kill.insert(dst);
    }

    void KillSetVisitor::visit(lir::InstructionCmp *i) { kill.clear(); }

    void KillSetVisitor::visit(lir::InstructionJmp *i) { kill.clear(); }

    void KillSetVisitor::visit(lir::InstructionCJmp *i) { kill.clear(); }

    // TODO: is this correct?
    void KillSetVisitor::visit(lir::InstructionCall *i) { kill.clear(); }

    // TODO: is this correct?
    void KillSetVisitor::visit(lir::InstructionRet *i) { kill.clear(); }

    void KillSetVisitor::visit(lir::InstructionPhi *i) {
        kill.clear();

        auto dst = dynamic_cast<lir::Register *>(i->getDst());
        if (dst)
            kill.insert(dst);
    }

    void KillSetVisitor::visit(lir::InstructionVirtual *i) {
        i->getInstruction()->accept(this);
    }

    void KillSetVisitor::visit(lir::InstructionUnknown *i) {}

    SuccessorVisitor::SuccessorVisitor(
        std::list<std::unique_ptr<lir::Instruction>> &instructions)
        : instructions(instructions) {}

    std::vector<int> SuccessorVisitor::getResult() { return successors; }

    void SuccessorVisitor::visit(lir::Instruction *i) {}

    void SuccessorVisitor::visit(lir::Label *l) {}

    void SuccessorVisitor::visit(lir::InstructionMov *i) {}

    void SuccessorVisitor::visit(lir::InstructionPush *i) {}

    void SuccessorVisitor::visit(lir::InstructionPop *i) {}

    void SuccessorVisitor::visit(lir::InstructionConvert *i) {}

    void SuccessorVisitor::visit(lir::InstructionBinaryOp *i) {}

    void SuccessorVisitor::visit(lir::InstructionSpecialOp *i) {}

    void SuccessorVisitor::visit(lir::InstructionLea *i) {}

    void SuccessorVisitor::visit(lir::InstructionCmp *i) {}

    void SuccessorVisitor::visit(lir::InstructionJmp *i) {}

    void SuccessorVisitor::visit(lir::InstructionCJmp *i) {}

    void SuccessorVisitor::visit(lir::InstructionCall *i) {}

    void SuccessorVisitor::visit(lir::InstructionRet *i) {}

    void SuccessorVisitor::visit(lir::InstructionPhi *i) {}

    void SuccessorVisitor::visit(lir::InstructionVirtual *i) {}

    void SuccessorVisitor::visit(lir::InstructionUnknown *i) {}

    Liveness::Liveness(lir::Program &p)
        : program(p), gsv(GenSetVisitor(p.getOm())),
          ksv(KillSetVisitor(p.getOm())), sv(p.getInstructions()) {}

    void Liveness::computeLiveRanges() {
        std::vector<RegisterSet> gen;
        std::vector<RegisterSet> kill;
        std::vector<std::vector<int>> successors;

        for (auto &i : program.getInstructions()) {
            i->accept(&gsv);
            i->accept(&ksv);

            gen.push_back(std::move(gsv.getResult()));
            kill.push_back(std::move(ksv.getResult()));
            successors.push_back(std::move(sv.getResult()));
        }

        auto size = program.getInstructions().size();
        in = std::vector<RegisterSet>(size);
        out = std::vector<RegisterSet>(size);

        RegisterSet new_in, new_out;
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < size; i++) {
                new_in = gen[i];
                for (auto *reg : out[i])
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

    void Liveness::printGenKill() {
        auto line = 0;
        for (auto &i : program.getInstructions()) {
            lir::ToStringVisitor tsv;
            i->accept(&tsv);
            std::cout << line++ << ": " << tsv.getResult() << std::endl;

            i->accept(&gsv);
            i->accept(&ksv);

            auto gen = gsv.getResult();
            auto kill = ksv.getResult();

            std::cout << "- gen:  ";
            for (auto *reg : gen)
                std::cout << reg->toString() << ", ";
            std::cout << std::endl;

            std::cout << "- kill: ";
            for (auto *reg : kill)
                std::cout << reg->toString() << ", ";
            std::cout << std::endl;
        }
    }

    std::vector<RegisterSet> Liveness::getIn() { return in; }

    std::vector<RegisterSet> Liveness::getOut() { return out; }
} // namespace backend
