#pragma once

#include "bulbit/transform.h"

namespace bulbit
{

class Mesh;
class Material;
class Scene;

bool GetAreaLightSourceCreationEnabled();
void SetAreaLightSourceCreationEnabled(bool enabled);

void CreateSphere(
    Scene& scene,
    Transform tf,
    Float radius,
    const Material* material,
    const MediumInterface& medium_interface = {},
    bool area_light = false
);
void CreateTriangles(
    Scene& scene,
    const Mesh* mesh,
    const Material* material,
    const MediumInterface& medium_interface = {},
    bool area_light = false
);

void CreateRectXY(
    Scene& scene,
    const Transform& transform,
    const Material* material,
    const MediumInterface& medium_interface = {},
    const Point2& tex_coord = Point2(1, 1),
    bool area_light = false
);
void CreateRectXZ(
    Scene& scene,
    const Transform& transform,
    const Material* material,
    const MediumInterface& medium_interface = {},
    const Point2& tex_coord = Point2(1, 1),
    bool area_light = false
);
void CreateRectYZ(
    Scene& scene,
    const Transform& transform,
    const Material* material,
    const MediumInterface& medium_interface = {},
    const Point2& tex_coord = Point2(1, 1),
    bool area_light = false
);
void CreateBox(
    Scene& scene,
    const Transform& transform,
    const Material* material,
    const MediumInterface& medium_interface = {},
    const Point2& tex_coord = Point2(1, 1),
    bool area_light = false
);

inline DiffuseMaterial* CreateDiffuseMaterial(Scene& scene, const Spectrum& sp)
{
    return scene.CreateMaterial<DiffuseMaterial>(scene.CreateConstantTexture(sp));
}

inline DiffuseMaterial* CreateDiffuseMaterial(Scene& scene, const Spectrum& sp, const SpectrumTexture* normalmap, Float alpha)
{
    return scene.CreateMaterial<DiffuseMaterial>(
        scene.CreateConstantTexture(sp), normalmap, scene.CreateConstantTexture<Float>(alpha)
    );
}

inline DielectricMaterial* CreateDielectricMaterial(
    Scene& scene, Float eta, Float roughness = 0, const SpectrumTexture* normalmap = nullptr
)
{
    return scene.CreateMaterial<DielectricMaterial>(
        eta, scene.CreateConstantTexture<Float>(roughness), scene.CreateConstantTexture<Float>(roughness), normalmap
    );
}

inline ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& eta, const Spectrum& k, Float roughness, const SpectrumTexture* normalmap = nullptr
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(eta), scene.CreateConstantTexture<Spectrum>(k),
        scene.CreateConstantTexture<Float>(roughness), scene.CreateConstantTexture<Float>(roughness), normalmap
    );
}

inline ConductorMaterial* CreateConductorMaterial(
    Scene& scene,
    const Spectrum& eta,
    const Spectrum& k,
    Float roughness_u,
    Float roughness_v,
    const SpectrumTexture* normalmap = nullptr
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(eta), scene.CreateConstantTexture<Spectrum>(k),
        scene.CreateConstantTexture<Float>(roughness_u), scene.CreateConstantTexture<Float>(roughness_v), normalmap
    );
}

inline ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& reflectance, Float roughness, const SpectrumTexture* normalmap = nullptr
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), scene.CreateConstantTexture<Float>(roughness),
        scene.CreateConstantTexture<Float>(roughness), normalmap
    );
}

inline ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& reflectance, Float roughness_u, Float roughness_v, const SpectrumTexture* normalmap = nullptr
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), scene.CreateConstantTexture<Float>(roughness_u),
        scene.CreateConstantTexture<Float>(roughness_v), normalmap
    );
}

inline UnrealMaterial* CreateUnrealMaterial(
    Scene& scene,
    const Spectrum& basecolor,
    Float metallic,
    Float roughness,
    const Spectrum& emission = Spectrum::black,
    const SpectrumTexture* normalmap = nullptr,
    const FloatTexture* alpha = nullptr
)
{
    return scene.CreateMaterial<UnrealMaterial>(
        scene.CreateConstantTexture<Spectrum>(basecolor), scene.CreateConstantTexture<Float>(metallic),
        scene.CreateConstantTexture<Float>(roughness), scene.CreateConstantTexture<Float>(roughness),
        scene.CreateConstantTexture<Spectrum>(emission), normalmap, alpha
    );
}

inline UnrealMaterial* CreateUnrealMaterial(
    Scene& scene,
    const Spectrum& basecolor,
    Float metallic,
    Float u_roughness,
    Float v_roughness,
    const Spectrum& emission = Spectrum::black,
    const SpectrumTexture* normalmap = nullptr,
    const FloatTexture* alpha = nullptr
)
{
    return scene.CreateMaterial<UnrealMaterial>(
        scene.CreateConstantTexture<Spectrum>(basecolor), scene.CreateConstantTexture<Float>(metallic),
        scene.CreateConstantTexture<Float>(u_roughness), scene.CreateConstantTexture<Float>(v_roughness),
        scene.CreateConstantTexture<Spectrum>(emission), normalmap, alpha
    );
}

inline SubsurfaceDiffusionMaterial* CreateSubsurfaceDiffusionMaterial(
    Scene& scene,
    const Spectrum& reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    const SpectrumTexture* normalmap = nullptr
)
{
    return scene.CreateMaterial<SubsurfaceDiffusionMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), mfp, eta, scene.CreateConstantTexture<Float>(roughness),
        scene.CreateConstantTexture<Float>(roughness), normalmap
    );
}

inline SubsurfaceRandomWalkMaterial* CreateSubsurfaceRandomWalkMaterial(
    Scene& scene,
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    Float g = 0,
    const SpectrumTexture* normalmap = nullptr
)
{
    return scene.CreateMaterial<SubsurfaceRandomWalkMaterial>(
        reflectance, mfp, eta, scene.CreateConstantTexture<Float>(roughness), scene.CreateConstantTexture<Float>(roughness), g,
        normalmap
    );
}

inline SubsurfaceRandomWalkMaterial* CreateSubsurfaceRandomWalkMaterial(
    Scene& scene,
    const Spectrum& reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    Float g = 0,
    const SpectrumTexture* normalmap = nullptr
)
{
    return scene.CreateMaterial<SubsurfaceRandomWalkMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), mfp, eta, scene.CreateConstantTexture<Float>(roughness),
        scene.CreateConstantTexture<Float>(roughness), g, normalmap
    );
}

inline MixtureMaterial* CreateMixtureMaterial(Scene& scene, const Material* material1, const Material* material2, Float amount)
{
    return scene.CreateMaterial<MixtureMaterial>(material1, material2, scene.CreateConstantTexture<Float>(amount));
}

inline MirrorMaterial* CreateMirrorMaterial(
    Scene& scene, const Spectrum& reflectance, const SpectrumTexture* normalmap = nullptr, Float alpha = 1
)
{
    return scene.CreateMaterial<MirrorMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), normalmap, scene.CreateConstantTexture<Float>(alpha)
    );
}

inline DiffuseLightMaterial* CreateDiffuseLightMaterial(
    Scene& scene, const Spectrum& color, bool two_sided = false, Float alpha = 1
)
{
    return scene.CreateMaterial<DiffuseLightMaterial>(
        scene.CreateConstantTexture<Spectrum>(color), two_sided, scene.CreateConstantTexture<Float>(alpha)
    );
}

inline ImageInfiniteLight* CreateImageInfiniteLight(Scene& scene, std::string filename, const Transform& tf = identity)
{
    return scene.CreateLight<ImageInfiniteLight>(scene.CreateImageTexture<Spectrum>(ReadImage3(filename, false)), tf);
}

} // namespace bulbit