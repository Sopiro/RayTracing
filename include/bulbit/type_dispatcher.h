#pragma once

#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <variant>

template <typename... Types>
class TypeDispatcher
{
public:
    // Bundle all possible types into a std::variant
    using VariantType = std::variant<Types...>;

    template <typename Type>
    std::size_t GetTypeIndex() const
    {
        return GetTypeIndexImpl<Type>(std::make_index_sequence<sizeof...(Types)>{});
    }

    // Cast pointer to VariantType based on runtime index
    VariantType Cast(std::size_t index, void* ptr) const
    {
        return CastImpl(index, ptr, std::make_index_sequence<sizeof...(Types)>{});
    }

    VariantType Cast(std::size_t index, const void* ptr) const
    {
        return CastConstImpl(index, ptr, std::make_index_sequence<sizeof...(Types)>{});
    }

    // Cast ptr into certain type and dispatch callcable
    template <typename Callable>
    auto Dispatch(std::size_t index, void* ptr, Callable&& callable) const
    {
        VariantType variant = Cast(index, ptr);
        return std::visit(std::forward<Callable>(callable), variant);
    }

    template <typename Callable>
    auto Dispatch(std::size_t index, const void* ptr, Callable&& callable) const
    {
        VariantType variant = Cast(index, ptr);
        return std::visit(std::forward<Callable>(callable), variant);
    }

private:
    // Casts a void* to the appropriate type based on index sequence
    template <std::size_t... Is>
    VariantType CastImpl(std::size_t index, void* ptr, std::index_sequence<Is...>) const
    {
        using CastFuncType = VariantType (*)(void*);
        static const CastFuncType casters[] = { +[](void* p) -> VariantType {
            return *static_cast<std::add_pointer_t<Types>>(p);
        }... };

        if (index >= sizeof...(Types))
        {
            throw std::out_of_range("Index out of range");
        }
        return casters[index](ptr);
    }

    // Casts a const void* to the appropriate type based on index sequence
    template <std::size_t... Is>
    VariantType CastConstImpl(std::size_t index, const void* ptr, std::index_sequence<Is...>) const
    {
        using CastFuncType = VariantType (*)(const void*);
        static const CastFuncType casters[] = { +[](const void* p) -> VariantType {
            return *static_cast<std::add_pointer_t<const Types>>(p);
        }... };

        if (index >= sizeof...(Types))
        {
            throw std::out_of_range("Index out of range");
        }
        return casters[index](ptr);
    }
};