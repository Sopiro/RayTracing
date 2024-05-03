#pragma once

#include "scene.h"

namespace bulbit
{

constexpr int32 ao_channel = 0;
constexpr int32 roughness_channel = 1;
constexpr int32 metallic_channel = 2;

void LoadModel(Scene& scene,
               const std::string& filename,
               const Transform& transform,
               const Material* fallback_material = nullptr);

} // namespace bulbit
