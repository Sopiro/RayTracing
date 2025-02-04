#pragma once

#include "dynamic_dispatcher.h"

namespace bulbit
{

template <typename Base>
class PolymorphicVector
{
private:
    using TypePack = typename Base::Types;

    std::array<std::vector<uint8>, TypePack::count> vectors;
    std::array<size_t, TypePack::count> counts = { 0 };

    template <typename T>
    static constexpr int32 TypeIndexOf()
    {
        return detail::IndexOf<T, TypePack>::value;
    }

public:
    struct Index : public DynamicDispatcher<TypePack>
    {
        const int32 element_index;

        Index(int32 type_index, int32 element_index)
            : DynamicDispatcher<TypePack>{ type_index }
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
        return index.Dispatch([&](auto dummy) {
            const int32 type_index = dummy->type_index;
            auto& vector = vectors[type_index];
            const size_t offset = index.element_index * sizeof(*dummy);
            using T = std::remove_pointer_t<decltype(dummy)>;
            return reinterpret_cast<Base*>(&vector[offset]);
        });
    }

    const Base* operator[](const Index& index) const
    {
        return operator[](index);
    }

    template <typename T>
    T* operator[](const TypedIndex<T>& index)
    {
        auto& vector = vectors[index.type_index];
        const size_t offset = index.element_index * sizeof(T);
        return reinterpret_cast<T*>(&vector[offset]);
    }

    template <typename T>
    const Base* operator[](const TypedIndex<T>& index) const
    {
        return operator[](index);
    }

private:
    static size_t AlignOffset(size_t offset, size_t alignment)
    {
        const size_t mod = offset % alignment;
        return mod == 0 ? offset : offset + (alignment - mod);
    }
};

} // namespace bulbit