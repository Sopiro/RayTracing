#pragma once

#include <stdexcept>
#include <type_traits>
#include <variant>

template <typename... Types>
class TypeDispatcher
{
protected:
    const size_t type_index;

    TypeDispatcher(size_t type_index)
        : type_index{ type_index }
    {
        if (type_index >= sizeof...(Types))
        {
            throw std::out_of_range("Index out of range");
        }
    }

public:
    // Bundle all possible types into a std::variant
    using VariantType = std::variant<Types...>;

    // Cast ptr into certain type and dispatch callcable
    template <typename Callable>
    auto Dispatch(Callable&& callable) const
    {
        VariantType variant = Cast();
        return std::visit(std::forward<Callable>(callable), variant);
    }

private:
    // Cast pointer to VariantType based on runtime index
    VariantType Cast() const
    {
        using CastFuncType = VariantType (*)(void*);
        static const CastFuncType casters[] = { +[](void* p) -> VariantType {
            return *static_cast<std::add_pointer_t<Types>>(p);
        }... };

        return casters[type_index]((void*)this);
    }
};