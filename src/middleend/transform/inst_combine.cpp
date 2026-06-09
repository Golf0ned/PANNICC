#include "middleend/transform/inst_combine.h"
#include "middleend/utils/replace_uses.h"

namespace middleend {

void InstCombine::run(mir::Program &p) {
    for (auto &f : p.getFunctions())
        run(f.get());
}

void InstCombine::run(mir::Function *f) {
    auto definition = dynamic_cast<mir::FunctionDefinition *>(f);
    if (!definition)
        return;

    bool changed = true;
    while (changed) {
        changed = false;
        std::vector<std::unique_ptr<mir::Instruction>> to_drop;
        for (auto &bb : definition->getBasicBlocks()) {
            auto &instructions = bb->getInstructions();
            auto iter = instructions.begin();
            while (iter != instructions.end()) {
                auto i = iter->get();

                auto tryConstantFold = [&]() {
                    auto bin_op = dynamic_cast<mir::InstructionBinaryOp *>(i);
                    if (!bin_op)
                        return false;

                    auto left = dynamic_cast<mir::Literal *>(bin_op->getLeft());
                    if (!left)
                        return false;

                    auto right =
                        dynamic_cast<mir::Literal *>(bin_op->getRight());
                    if (!right)
                        return false;

                    int32_t lhs = static_cast<int32_t>(
                        static_cast<uint32_t>(left->getValue()));
                    int32_t rhs = static_cast<int32_t>(
                        static_cast<uint32_t>(right->getValue()));

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
                    case mir::BinaryOp::SDIV:
                        folded_val = lhs / rhs;
                        break;
                    case mir::BinaryOp::AND:
                        folded_val = lhs & rhs;
                        break;
                    case mir::BinaryOp::OR:
                        folded_val = lhs | rhs;
                        break;
                    case mir::BinaryOp::XOR:
                        folded_val = lhs ^ rhs;
                        break;
                    case mir::BinaryOp::SHL:
                        folded_val = lhs << rhs;
                        break;
                    case mir::BinaryOp::ASHR:
                        folded_val = lhs >> rhs;
                        break;
                    default:
                        return false;
                    }

                    auto type = mir::Type::I32;
                    auto extended_val =
                        static_cast<uint64_t>(static_cast<int64_t>(folded_val));
                    auto folded_literal =
                        mir::LiteralMap::getLiteral(type, extended_val);

                    replaceUses(bin_op, folded_literal);
                    to_drop.push_back(std::move(*iter));
                    iter = instructions.erase(iter);
                    return true;
                };

                auto trySimplify = [&]() {
                    auto bin_op = dynamic_cast<mir::InstructionBinaryOp *>(i);
                    if (!bin_op)
                        return false;

                    auto op = bin_op->getOp();
                    auto left = bin_op->getLeft();
                    auto right = bin_op->getRight();
                    auto left_literal = dynamic_cast<mir::Literal *>(left);
                    auto right_literal = dynamic_cast<mir::Literal *>(right);

                    // TODO: clean everything below up
                    if (!left_literal ^ !!right_literal)
                        return false;

                    auto constant = left_literal ? left_literal : right_literal;
                    auto var = left_literal ? right : left;

                    if (right_literal) {
                        switch (op) {
                        default:
                            break;
                        case mir::BinaryOp::SUB:
                            if (constant->getValue() == 0) {
                                replaceUses(bin_op, var);
                                to_drop.push_back(std::move(*iter));
                                iter = instructions.erase(iter);
                                return true;
                            }
                            break;
                        case mir::BinaryOp::SHL:
                        case mir::BinaryOp::ASHR:
                            if (constant->getValue() == 0) {
                                replaceUses(bin_op, var);
                                to_drop.push_back(std::move(*iter));
                                iter = instructions.erase(iter);
                                return true;
                            }
                            // TODO: check for past var size (UB)
                            break;
                        case mir::BinaryOp::SDIV:
                            if (constant->getValue() == 1) {
                                replaceUses(bin_op, var);
                                to_drop.push_back(std::move(*iter));
                                iter = instructions.erase(iter);
                                return true;
                            }
                            break;
                        }
                    }

                    switch (op) {
                    default:
                        break;
                    case mir::BinaryOp::ADD:
                    case mir::BinaryOp::OR:
                        if (constant->getValue() == 0) {
                            replaceUses(bin_op, var);
                            to_drop.push_back(std::move(*iter));
                            iter = instructions.erase(iter);
                            return true;
                        }
                        break;
                    case mir::BinaryOp::MUL:
                        if (constant->getValue() == 0) {
                            replaceUses(bin_op, constant);
                            to_drop.push_back(std::move(*iter));
                            iter = instructions.erase(iter);
                            return true;
                        } else if (constant->getValue() == 1) {
                            replaceUses(bin_op, var);
                            to_drop.push_back(std::move(*iter));
                            iter = instructions.erase(iter);
                            return true;
                        }
                        break;
                    case mir::BinaryOp::AND:
                        if (constant->getValue() == -1) {
                            replaceUses(bin_op, var);
                            to_drop.push_back(std::move(*iter));
                            iter = instructions.erase(iter);
                            return true;
                        }
                        break;
                    }

                    return false;
                };

                if (false
                    // clang-format off
                        || tryConstantFold()
                        || trySimplify()
                    // clang-format on
                ) {
                    changed = true;
                    continue;
                }

                iter++;
            }
        }
    }
}

} // namespace middleend
