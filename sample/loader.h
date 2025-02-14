#pragma once

#include "bulbit/bulbit.h"

namespace bulbit
{

constexpr int32 ao_channel = 0;
constexpr int32 roughness_channel = 1;
constexpr int32 metallic_channel = 2;
constexpr int32 alpha_channel = 3;

void SetLoaderFlipNormal(bool flip_normal);
void SetLoaderFlipTexcoord(bool flip_texcoord);
void SetLoaderUseForceFallbackMaterial(bool force_use_fallback_material);
void SetLoaderFallbackMaterial(const Material* fallback_material);
void SetLoaderFallbackMediumInterface(const MediumInterface& fallback_medium_interface);
void LoadModel(Scene& scene, std::filesystem::path filename, const Transform& transform);

} // namespace bulbit
