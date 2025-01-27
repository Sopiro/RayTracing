#pragma once

#include "bulbit/materials.h"
#include "bulbit/scene.h"

namespace bulbit
{

DiffuseMaterial* CreateDiffuseMaterial(
    Scene& scene, Float reflectance, const SpectrumTexture* normalmap = nullptr, Float alpha = 1
);
DiffuseMaterial* CreateDiffuseMaterial(
    Scene& scene, const Spectrum& reflectance, const SpectrumTexture* normalmap = nullptr, Float alpha = 1
);
DiffuseMaterial* CreateDiffuseMaterial(
    Scene& scene, const std::string& filename, const SpectrumTexture* normalmap = nullptr, Float alpha = 1
);

DielectricMaterial* CreateDielectricMaterial(
    Scene& scene, Float eta, Float roughness = 0, const SpectrumTexture* normalmap = nullptr
);

ConductorMaterial* CreateConductorMaterial(
    Scene& scene,
    const Spectrum& eta,
    const Spectrum& k,
    Float roughness,
    const SpectrumTexture* normalmap = nullptr,
    Float alpha = 1
);
ConductorMaterial* CreateConductorMaterial(
    Scene& scene,
    const Spectrum& eta,
    const Spectrum& k,
    Float roughness_u,
    Float roughness_v,
    const SpectrumTexture* normalmap = nullptr,
    Float alpha = 1
);
ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& reflectance, Float roughness, const SpectrumTexture* normalmap = nullptr, Float alpha = 1
);
ConductorMaterial* CreateConductorMaterial(
    Scene& scene,
    const Spectrum& reflectance,
    Float roughness_u,
    Float roughness_v,
    const SpectrumTexture* normalmap = nullptr,
    Float alpha = 1
);

UnrealMaterial* CreateUnrealMaterial(
    Scene& scene,
    const Spectrum& basecolor,
    Float metallic,
    Float roughness,
    const Spectrum& emission = Spectrum::black,
    const SpectrumTexture* normalmap = nullptr,
    const FloatTexture* alpha = nullptr
);
UnrealMaterial* CreateUnrealMaterial(
    Scene& scene,
    const Spectrum& basecolor,
    Float metallic,
    Float u_roughness,
    Float v_roughness,
    const Spectrum& emission = Spectrum::black,
    const SpectrumTexture* normalmap = nullptr,
    const FloatTexture* alpha = nullptr
);

SubsurfaceDiffusionMaterial* CreateSubsurfaceDiffusionMaterial(
    Scene& scene,
    const Spectrum& reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    const SpectrumTexture* normalmap = nullptr
);

SubsurfaceDiffusionMaterial* CreateSubsurfaceDiffusionMaterial(
    Scene& scene,
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    const SpectrumTexture* normalmap = nullptr
);

SubsurfaceRandomWalkMaterial* CreateSubsurfaceRandomWalkMaterial(
    Scene& scene,
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    Float g = 0,
    const SpectrumTexture* normalmap = nullptr
);
SubsurfaceRandomWalkMaterial* CreateSubsurfaceRandomWalkMaterial(
    Scene& scene,
    const Spectrum& reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    Float g = 0,
    const SpectrumTexture* normalmap = nullptr
);

MixtureMaterial* CreateMixtureMaterial(Scene& scene, const Material* material1, const Material* material2, Float amount);

MirrorMaterial* CreateMirrorMaterial(
    Scene& scene, const Spectrum& reflectance, const SpectrumTexture* normalmap = nullptr, Float alpha = 1
);

DiffuseLightMaterial* CreateDiffuseLightMaterial(Scene& scene, Float emission, bool two_sided = false, Float alpha = 1);
DiffuseLightMaterial* CreateDiffuseLightMaterial(Scene& scene, const Spectrum& emission, bool two_sided = false, Float alpha = 1);

const Material* CreateRandomUnrealMaterial(Scene& scene);

} // namespace bulbit
