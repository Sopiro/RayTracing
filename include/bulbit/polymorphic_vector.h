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

    // Iterator classes
    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Base*;
        using difference_type = std::ptrdiff_t;
        using pointer = Base**;
        using reference = Base*&;

        iterator(PolymorphicVector* pv, int32 type_index, int32 element_index)
            : pv{ pv }
            , ti{ type_index }
            , ei{ element_index }
        {
            advance();
        }

        Base* operator*() const
        {
            if (!pv || ti >= TypePack::count) return nullptr;
            return (*pv)[Index{ ti, ei }];
        }

        iterator& operator++()
        {
            if (!pv || ti >= TypePack::count) return *this;
            ++ei;
            advance();
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const
        {
            return pv == other.pv && ti == other.ti && ei == other.ei;
        }

        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }

    private:
        void advance()
        {
            while (ti < TypePack::count && ei >= pv->counts[ti])
            {
                ++ti;
                ei = 0;
            }
        }

        PolymorphicVector* pv;
        int ti, ei;
    };

    class const_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const Base*;
        using difference_type = std::ptrdiff_t;
        using pointer = const Base**;
        using reference = const Base*&;

        const_iterator(const PolymorphicVector* pv, int type_index, int element_index)
            : pv{ pv }
            , ti{ type_index }
            , ei{ element_index }
        {
            advance();
        }

        const Base* operator*() const
        {
            if (!pv || ti >= TypePack::count) return nullptr;
            return (*pv)[Index{ ti, ei }];
        }

        const_iterator& operator++()
        {
            if (!pv || ti >= TypePack::count) return *this;
            ++ei;
            advance();
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const const_iterator& other) const
        {
            return pv == other.pv && ti == other.ti && ei == other.ei;
        }

        bool operator!=(const const_iterator& other) const
        {
            return !(*this == other);
        }

    private:
        void advance()
        {
            while (ti < TypePack::count && ei >= pv->counts[ti])
            {
                ++ti;
                ei = 0;
            }
        }

        const PolymorphicVector* pv;
        int32 ti, ei;
    };

    PolymorphicVector()
        : vectors{}
        , counts{ 0 }
    {
    }

    // Reserve space for 'capacity' elements of type T
    template <typename T>
    void reserve(size_t capacity)
    {
        constexpr int32 type_index = TypeIndexOf<T>();
        vectors[type_index].reserve(capacity * sizeof(T));
    }

    template <typename T>
    void resize(size_t size)
    {
        constexpr int32 type_index = TypeIndexOf<T>();
        vectors[type_index].resize(size);
    }

    template <typename T>
    void shrink_to_fit(size_t size)
    {
        constexpr int32 type_index = TypeIndexOf<T>();
        vectors[type_index].shrink_to_fit();
    }

    void clear()
    {
        for (auto v : vectors)
        {
            v.clear();
        }
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

    // Iterator functions
    iterator begin()
    {
        return iterator(this, 0, 0);
    }

    iterator end()
    {
        return iterator(this, TypePack::count, 0);
    }

    const_iterator begin() const
    {
        return const_iterator(this, 0, 0);
    }

    const_iterator end() const
    {
        return const_iterator(this, TypePack::count, 0);
    }
};

} // namespace bulbit