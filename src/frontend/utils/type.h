#pragma once

#include "middleend/mir/type.h"

#include <memory>
#include <string>

namespace frontend {

class Type {
public:
    virtual std::string toString() = 0;
    virtual middleend::mir::Type toMir() = 0;
    virtual std::unique_ptr<Type> clone() = 0;
    virtual ~Type() = default;
};

class Int : public Type {
public:
    std::string toString() override;
    middleend::mir::Type toMir() override;
    std::unique_ptr<Type> clone() override;
};

class Ptr : public Type {
public:
    Ptr(std::unique_ptr<Type> base);
    Type *getBase();
    std::string toString() override;
    middleend::mir::Type toMir() override;
    std::unique_ptr<Type> clone() override;

private:
    std::unique_ptr<Type> base;
};

} // namespace frontend
