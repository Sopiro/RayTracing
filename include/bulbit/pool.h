#pragma once

#include "common.h"

namespace bulbit
{

template <typename Key, typename Type, typename Hash = std::hash<Key>>
class Pool
{
public:
    Pool();
    ~Pool();

    template <typename... Args>
    Type* Create(const Key& key, Args&&... args);
    bool Delete(const Key& key);
    void Clear();

    std::unordered_map<Key, Type*, Hash>& GetObjectMap();
    int32 PoolCount() const;

private:
    std::pmr::monotonic_buffer_resource resource;
    std::pmr::polymorphic_allocator<std::byte> allocator;

    int32 count = 0;
    std::unordered_map<Key, Type*, Hash> objects;
};

template <typename Key, typename Type, typename Hash>
inline Pool<Key, Type, Hash>::Pool()
    : resource{}
    , allocator{ &resource }
{
}

template <typename Key, typename Type, typename Hash>
inline Pool<Key, Type, Hash>::~Pool()
{
    Clear();
}

template <typename Key, typename Type, typename Hash>
template <typename... Args>
inline Type* Pool<Key, Type, Hash>::Create(const Key& key, Args&&... args)
{
    auto it = objects.find(key);
    if (it != objects.end())
    {
        // Already in pool
        return it->second;
    }

    // Create new object using pool allocator
    Type* object = allocator.new_object<Type>(key, std::forward<Args>(args)...);

    objects.emplace(key, object);
    ++count;

    return object;
}

template <typename Key, typename Type, typename Hash>
inline bool Pool<Key, Type, Hash>::Delete(const Key& key)
{
    auto it = objects.find(key);
    if (it != objects.end())
    {
        allocator.delete_object(it->second);
        objects.erase(it);

        return true;
    }

    return false;
}

template <typename Key, typename Type, typename Hash>
inline void Pool<Key, Type, Hash>::Clear()
{
    for (auto& kv : objects)
    {
        allocator.delete_object(kv.second);
    }

    objects.clear();
}

template <typename Key, typename Type, typename Hash>
inline std::unordered_map<Key, Type*, Hash>& Pool<Key, Type, Hash>::GetObjectMap()
{
    return objects;
}

template <typename Key, typename Type, typename Hash>
inline int32 Pool<Key, Type, Hash>::PoolCount() const
{
    return count;
}

} // namespace bulbit
