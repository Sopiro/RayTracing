#pragma once

#include "scene.h"

namespace bulbit
{

void LoadModel(Scene& scene,
               const std::string& filename,
               const Transform& transform,
               const Material* fallback_material = nullptr);

} // namespace bulbit
