#include <utility>

#include "frontend/utils/type.h"

namespace frontend {

    std::string Int::toString() { return "int"; }

    middleend::mir::Type Int::toMir() { return middleend::mir::Type::I32; }

    Ptr::Ptr(std::unique_ptr<Type> base) : base(std::move(base)) {}

    std::unique_ptr<Type> &Ptr::getBase() { return base; }

    std::string Ptr::toString() {
        auto base_ptr = dynamic_cast<Ptr *>(base.get());
        return base->toString() + (base_ptr ? "" : " ") + "*";
    }

    middleend::mir::Type Ptr::toMir() { return middleend::mir::Type::PTR; }
} // namespace frontend
