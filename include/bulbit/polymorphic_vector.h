#pragma once

#include "dynamic_dispatcher.h"

namespace bulbit
{

// Primary template with Base and TypePack defaulting to Base::Types
template <typename Base, typename TypePack = typename Base::Types>
class PolymorphicVector;

// Specialization for TypePack<Types...>
template <typename Base, typename... Types>
class PolymorphicVector<Base, TypePack<Types...>>
{
private:
    using TypePack = TypePack<Types...>;

    std::array<std::vector<uint8>, TypePack::count> vectors;
    std::array<size_t, TypePack::count> counts;

    template <typename T>
    static constexpr int32 TypeIndexOf()
    {
        return detail::IndexOf<T, TypePack>::value;
    }

public:
    struct Index
    {
        int32 type_index;
        int32 element_index;

        Index()
            : type_index{ -1 }
            , element_index{ -1 }
        {
        }

        Index(int32 type_index, int32 element_index)
            : type_index{ type_index }
            , element_index{ element_index }
        {
        }

        operator bool() const
        {
            return type_index >= 0 && element_index >= 0;
        }
        bool operator==(const Index& o) const = default;
        bool operator!=(const Index& o) const = default;
    };

    template <typename T>
    struct TypedIndex
    {
        static constexpr int32 type_index = TypeIndexOf<T>();
        int32 element_index;

        TypedIndex(int32 element_index)
            : element_index{ element_index }
        {
        }

        operator Index()
        {
            return { type_index, element_index };
        }
    };

    PolymorphicVector()
        : vectors{}
        , counts{ 0 }
    {
    }

    template <typename T>
    TypedIndex<T> push_back(const T& value)
    {
        return emplace_back<T>(value);
    }

    template <typename T, typename... Args>
    TypedIndex<T> emplace_back(Args&&... args)
    {
        constexpr int32 type_index = TypeIndexOf<T>();
        auto& vector = vectors[type_index];
        const size_t offset = vector.size();

        vector.resize(offset + sizeof(T));
        new (&vector[offset]) T(std::forward<Args>(args)...);
        counts[type_index]++;

        return { static_cast<int32>(counts[type_index] - 1) };
    }

    Base* operator[](const Index& index)
    {
        using Handler = Base* (*)(uint8*, int32);
        static constexpr Handler handlers[] = { [](uint8* v, int32 element_index) -> Base* {
            return reinterpret_cast<Types*>(v) + element_index;
        }... };

        return handlers[index.type_index](vectors[index.type_index].data(), index.element_index);
    }

    const Base* operator[](const Index& index) const
    {
        return const_cast<PolymorphicVector*>(this)->operator[](index);
    }

    template <typename T>
    T* operator[](const TypedIndex<T>& index)
    {
        return reinterpret_cast<T*>(vectors[index.type_index].data()) + index.element_index;
    }

    template <typename T>
    const T* operator[](const TypedIndex<T>& index) const
    {
        return const_cast<PolymorphicVector*>(this)->operator[](index);
    }
};

} // namespace bulbit