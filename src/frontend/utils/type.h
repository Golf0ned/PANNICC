#pragma once

#include <memory>
#include <string>

#include "middleend/mir/type.h"

namespace frontend {
    class Type {
    public:
        virtual std::string toString() = 0;
        virtual middleend::mir::Type toMir() = 0;
        virtual ~Type() = default;
    };

    class Int : Type {
    public:
        std::string toString() override;
        middleend::mir::Type toMir() override;
    };

    class Ptr : Type {
    public:
        Ptr(std::unique_ptr<Type> base);
        std::unique_ptr<Type> &getBase();
        std::string toString() override;
        middleend::mir::Type toMir() override;

    private:
        std::unique_ptr<Type> base;
    };
} // namespace frontend
