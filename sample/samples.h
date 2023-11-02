#pragma once

#include "bulbit/camera.h"
#include "bulbit/scene.h"

#include <unordered_map>

namespace bulbit
{

struct Sample
{
    using Func = Camera* (*)(Scene&);

    static int32 Register(std::string name, Func func);
    static bool Get(std::string name, Scene* scene, Camera** camera);

    static inline std::unordered_map<std::string, Func> samples;
    static inline int32 count = 0;
};

inline int32 Sample::Register(std::string name, Func func)
{
    samples.insert(std::make_pair(name, func));

    return ++count;
}

inline bool Sample::Get(std::string name, Scene* scene, Camera** camera)
{
    if (!samples.contains(name))
    {
        return false;
    }

    return *camera = samples.at(name)(*scene);
}

} // namespace bulbit
