#pragma once

#include "common.h"

namespace spt
{

// Inspired by b2GrowableStack in box2d code
template <typename T, int32 N>
class GrowableArray
{
public:
    GrowableArray()
        : array{ stackArray }
        , count{ 0 }
        , capacity{ N }
    {
    }

    ~GrowableArray() noexcept
    {
        if (array != stackArray)
        {
            std::free(array);
            array = nullptr;
        }
    }

    GrowableArray(const GrowableArray& other) noexcept
    {
        if (other.array == other.stackArray)
        {
            array = stackArray;
            memcpy(stackArray, other.stackArray, other.count * sizeof(T));
        }
        else
        {
            array = (T*)std::malloc(other.capacity * sizeof(T));
            memcpy(array, other.array, other.count * sizeof(T));
        }

        capacity = other.capacity;
        count = other.count;
    }

    GrowableArray& operator=(const GrowableArray& other) noexcept
    {
        assert(this != &other);

        if (array != stackArray)
        {
            std::free(array);
        }

        if (other.array == other.stackArray)
        {
            array = stackArray;
            memcpy(stackArray, other.stackArray, other.count * sizeof(T));
        }
        else
        {
            array = (T*)std::malloc(other.capacity * sizeof(T));
            memcpy(array, other.array, other.count * sizeof(T));
        }

        capacity = other.capacity;
        count = other.count;

        return *this;
    }

    GrowableArray(GrowableArray&& other) noexcept
    {
        if (other.array == other.stackArray)
        {
            array = stackArray;
            memcpy(stackArray, other.stackArray, other.count * sizeof(T));
        }
        else
        {
            array = other.array;
        }

        capacity = other.capacity;
        count = other.count;

        other.array = other.stackArray;
        other.count = 0;
        other.capacity = N;
    }

    GrowableArray& operator=(GrowableArray&& other) noexcept
    {
        assert(this != &other);

        if (array != stackArray)
        {
            std::free(array);
        }

        if (other.array == other.stackArray)
        {
            array = stackArray;
            memcpy(stackArray, other.stackArray, other.count * sizeof(T));
        }
        else
        {
            array = other.array;
        }

        capacity = other.capacity;
        count = other.count;

        other.array = other.stackArray;
        other.count = 0;
        other.capacity = N;

        return *this;
    }

    template <typename... Args>
    T& Emplace(Args&&... args)
    {
        if (count == capacity)
        {
            T* old = array;
            capacity *= 2;

            array = (T*)std::malloc(capacity * sizeof(T));
            memcpy(array, old, count * sizeof(T));

            if (old != stackArray)
            {
                std::free(old);
            }
        }

        return *new (array + count++) T{ std::forward<Args>(args)... };
    }

    void Push(const T& data)
    {
        Emplace(data);
    }

    void Push(T&& data)
    {
        Emplace(std::move(data));
    }

    T Pop()
    {
        assert(count > 0);
        --count;
        return array[count];
    }

    T& Back() const
    {
        return array[count - 1];
    }

    // O(n)
    void Insert(int32 index, const T& data)
    {
        assert(index <= count);

        if (count == capacity)
        {
            T* old = array;
            capacity *= 2;

            array = (T*)std::malloc(capacity * sizeof(T));
            memcpy(array, old, count * sizeof(T));

            if (old != stackArray)
            {
                std::free(old);
            }
        }

        int32 ptr = count;
        while (index != ptr)
        {
            array[ptr] = array[ptr - 1];
            --ptr;
        }

        array[index] = data;
        ++count;
    }

    // O(n)
    void Remove(int32 index)
    {
        int32 ptr = index;
        while (ptr != count)
        {
            array[ptr] = array[ptr + 1];
            ++ptr;
        }

        --count;
    }

    // O(1)
    void RemoveSwap(int32 index)
    {
        array[index] = array[count - 1];
        --count;
    }

    int32 Count() const
    {
        return count;
    }

    void Clear()
    {
        count = 0;
    }

    void Reset()
    {
        if (array != stackArray)
        {
            std::free(array);
        }
        array = stackArray;
        count = 0;
    }

    int32 Capacity() const
    {
        return capacity;
    }

    T& At(int32 index) const
    {
        return array[index];
    }

    T& operator[](int32 index) const
    {
        return array[index];
    }

private:
    T* array;
    T stackArray[N];
    int32 count;
    int32 capacity;
};

} // namespace spt
