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
    std::array<size_t, TypePack::count> counts = { 0 };

    template <typename T>
    static constexpr int32 TypeIndexOf()
    {
        return detail::IndexOf<T, TypePack>::value;
    }

public:
    struct Index
    {
        const int32 type_index;
        const int32 element_index;

        Index(int32 type_index, int32 element_index)
            : type_index{ type_index }
            , element_index{ element_index }
        {
        }
    };

    template <typename T>
    struct TypedIndex
    {
        static constexpr int32 type_index = TypeIndexOf<T>();
        const int32 element_index;

        TypedIndex(int32 element_index)
            : element_index{ element_index }
        {
        }

        operator Index()
        {
            return { type_index, element_index };
        }
    };

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
        auto& count = counts[type_index];

        constexpr size_t align = alignof(T);
        const size_t offset = AlignOffset(vector.size(), align);

        vector.resize(offset + sizeof(T));
        new (&vector[offset]) T(std::forward<Args>(args)...);
        count++;

        return TypedIndex<T>{ int32(count - 1) };
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
        return operator[](index);
    }

    template <typename T>
    T* operator[](const TypedIndex<T>& index)
    {
        return reinterpret_cast<T*>(vectors[index.type_index].data()) + index.element_index;
    }

    template <typename T>
    const T* operator[](const TypedIndex<T>& index) const
    {
        return operator[](index);
    }

private:
    static size_t AlignOffset(size_t offset, size_t alignment)
    {
#if 0 
        const size_t mod = offset % alignment;
        return mod == 0 ? offset : offset + (alignment - mod);
#else
        return (offset + alignment - 1) & ~(alignment - 1);
#endif
    }
};

} // namespace bulbit