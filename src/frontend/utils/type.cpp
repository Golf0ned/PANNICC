#include "frontend/utils/type.h"

#include <utility>

using namespace frontend;

std::string Int::toString() { return "int"; }

middleend::mir::Type Int::toMir() { return middleend::mir::Type::I32; }

std::unique_ptr<Type> Int::clone() { return std::make_unique<Int>(); }

Ptr::Ptr(std::unique_ptr<Type> base) : base(std::move(base)) {}

Type *Ptr::getBase() { return base.get(); }

std::string Ptr::toString() {
    auto base_ptr = dynamic_cast<Ptr *>(base.get());
    return base->toString() + (base_ptr ? "" : " ") + "*";
}

middleend::mir::Type Ptr::toMir() { return middleend::mir::Type::PTR; }

std::unique_ptr<Type> Ptr::clone() {
    return std::make_unique<Ptr>(base->clone());
}
