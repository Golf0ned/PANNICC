#pragma once

#include "middleend/mir/type.h"

#include <cstdint>
#include <memory>
#include <unordered_map>

namespace middleend::mir {

class Instruction;

// TODO: function params
class Value {
public:
    Value(Type type);
    Type getType();
    std::unordered_map<Instruction *, uint64_t> &getUses();
    virtual ~Value() = default;

private:
    Type type;
    std::unordered_map<Instruction *, uint64_t> uses;
};

class Literal : public Value {
public:
    Literal(Type type, int64_t value);
    int64_t getValue();

private:
    int64_t value;
};

class LiteralMap {
public:
    static Literal *getLiteral(Type type, uint64_t value);

private:
    static std::unordered_map<
        Type, std::unordered_map<uint64_t, std::unique_ptr<Literal>>>
        literals;
};

} // namespace middleend::mir
