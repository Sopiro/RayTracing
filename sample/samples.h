#pragma once

#include "bulbit/bulbit.h"

#include "light_builder.h"
#include "loader.h"
#include "material_builder.h"
#include "scene_builder.h"
#include "texture_builder.h"

#include <unordered_map>

using namespace bulbit;

struct Sample
{
    typedef std::unique_ptr<Camera> Func(Scene&);

    static int32 Register(std::string name, Func* func);
    static bool Get(std::string name, Scene* scene, std::unique_ptr<Camera>* camera);

    static inline std::unordered_map<std::string, Func*> samples;
    static inline int32 count = 0;
};

inline int32 Sample::Register(std::string name, Func* func)
{
    samples.insert(std::make_pair(name, func));

    return ++count;
}

inline bool Sample::Get(std::string name, Scene* scene, std::unique_ptr<Camera>* camera)
{
    if (!samples.contains(name))
    {
        return false;
    }

    *camera = samples.at(name)(*scene);
    return true;
}
