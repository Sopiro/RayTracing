#pragma once

#include "bulbit/scene.h"

namespace bulbit
{

constexpr int32 ao_channel = 0;
constexpr int32 roughness_channel = 1;
constexpr int32 metallic_channel = 2;

void SetLoaderFlipNormal(bool flip_normal);
void SetLoaderFallbackMaterial(const Material* fallback_material);
void LoadModel(Scene& scene, const std::string& filename, const Transform& transform);

} // namespace bulbit
