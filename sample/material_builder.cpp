#include "material_builder.h"

namespace bulbit
{

DiffuseMaterial* CreateDiffuseMaterial(Scene& scene, const Spectrum& sp)
{
    return scene.CreateMaterial<DiffuseMaterial>(scene.CreateConstantTexture(sp));
}

DiffuseMaterial* CreateDiffuseMaterial(Scene& scene, const Spectrum& sp, const SpectrumTexture* normalmap, Float alpha)
{
    return scene.CreateMaterial<DiffuseMaterial>(
        scene.CreateConstantTexture(sp), normalmap, scene.CreateConstantTexture<Float>(alpha)
    );
}

DielectricMaterial* CreateDielectricMaterial(Scene& scene, Float eta, Float roughness, const SpectrumTexture* normalmap)
{
    return scene.CreateMaterial<DielectricMaterial>(
        eta, scene.CreateConstantTexture<Float>(roughness), scene.CreateConstantTexture<Float>(roughness), normalmap
    );
}

ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& eta, const Spectrum& k, Float roughness, const SpectrumTexture* normalmap
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(eta), scene.CreateConstantTexture<Spectrum>(k),
        scene.CreateConstantTexture<Float>(roughness), scene.CreateConstantTexture<Float>(roughness), normalmap
    );
}

ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& eta, const Spectrum& k, Float roughness_u, Float roughness_v, const SpectrumTexture* normalmap
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(eta), scene.CreateConstantTexture<Spectrum>(k),
        scene.CreateConstantTexture<Float>(roughness_u), scene.CreateConstantTexture<Float>(roughness_v), normalmap
    );
}

ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& reflectance, Float roughness, const SpectrumTexture* normalmap
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), scene.CreateConstantTexture<Float>(roughness),
        scene.CreateConstantTexture<Float>(roughness), normalmap
    );
}

ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& reflectance, Float roughness_u, Float roughness_v, const SpectrumTexture* normalmap
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), scene.CreateConstantTexture<Float>(roughness_u),
        scene.CreateConstantTexture<Float>(roughness_v), normalmap
    );
}

UnrealMaterial* CreateUnrealMaterial(
    Scene& scene,
    const Spectrum& basecolor,
    Float metallic,
    Float roughness,
    const Spectrum& emission,
    const SpectrumTexture* normalmap,
    const FloatTexture* alpha
)
{
    return scene.CreateMaterial<UnrealMaterial>(
        scene.CreateConstantTexture<Spectrum>(basecolor), scene.CreateConstantTexture<Float>(metallic),
        scene.CreateConstantTexture<Float>(roughness), scene.CreateConstantTexture<Float>(roughness),
        scene.CreateConstantTexture<Spectrum>(emission), normalmap, alpha
    );
}

UnrealMaterial* CreateUnrealMaterial(
    Scene& scene,
    const Spectrum& basecolor,
    Float metallic,
    Float u_roughness,
    Float v_roughness,
    const Spectrum& emission,
    const SpectrumTexture* normalmap,
    const FloatTexture* alpha
)
{
    return scene.CreateMaterial<UnrealMaterial>(
        scene.CreateConstantTexture<Spectrum>(basecolor), scene.CreateConstantTexture<Float>(metallic),
        scene.CreateConstantTexture<Float>(u_roughness), scene.CreateConstantTexture<Float>(v_roughness),
        scene.CreateConstantTexture<Spectrum>(emission), normalmap, alpha
    );
}

SubsurfaceDiffusionMaterial* CreateSubsurfaceDiffusionMaterial(
    Scene& scene, const Spectrum& reflectance, const Spectrum& mfp, Float eta, Float roughness, const SpectrumTexture* normalmap
)
{
    return scene.CreateMaterial<SubsurfaceDiffusionMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), mfp, eta, scene.CreateConstantTexture<Float>(roughness),
        scene.CreateConstantTexture<Float>(roughness), normalmap
    );
}

SubsurfaceDiffusionMaterial* CreateSubsurfaceDiffusionMaterial(
    Scene& scene,
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    const SpectrumTexture* normalmap
)
{
    return scene.CreateMaterial<SubsurfaceDiffusionMaterial>(
        reflectance, mfp, eta, scene.CreateConstantTexture<Float>(roughness), scene.CreateConstantTexture<Float>(roughness),
        normalmap
    );
}

SubsurfaceRandomWalkMaterial* CreateSubsurfaceRandomWalkMaterial(
    Scene& scene,
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    Float g,
    const SpectrumTexture* normalmap
)
{
    return scene.CreateMaterial<SubsurfaceRandomWalkMaterial>(
        reflectance, mfp, eta, scene.CreateConstantTexture<Float>(roughness), scene.CreateConstantTexture<Float>(roughness), g,
        normalmap
    );
}

SubsurfaceRandomWalkMaterial* CreateSubsurfaceRandomWalkMaterial(
    Scene& scene,
    const Spectrum& reflectance,
    const Spectrum& mfp,
    Float eta,
    Float roughness,
    Float g,
    const SpectrumTexture* normalmap
)
{
    return scene.CreateMaterial<SubsurfaceRandomWalkMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), mfp, eta, scene.CreateConstantTexture<Float>(roughness),
        scene.CreateConstantTexture<Float>(roughness), g, normalmap
    );
}

MixtureMaterial* CreateMixtureMaterial(Scene& scene, const Material* material1, const Material* material2, Float amount)
{
    return scene.CreateMaterial<MixtureMaterial>(material1, material2, scene.CreateConstantTexture<Float>(amount));
}

MirrorMaterial* CreateMirrorMaterial(Scene& scene, const Spectrum& reflectance, const SpectrumTexture* normalmap, Float alpha)
{
    return scene.CreateMaterial<MirrorMaterial>(
        scene.CreateConstantTexture<Spectrum>(reflectance), normalmap, scene.CreateConstantTexture<Float>(alpha)
    );
}

DiffuseLightMaterial* CreateDiffuseLightMaterial(Scene& scene, const Spectrum& color, bool two_sided, Float alpha)
{
    return scene.CreateMaterial<DiffuseLightMaterial>(
        scene.CreateConstantTexture<Spectrum>(color), two_sided, scene.CreateConstantTexture<Float>(alpha)
    );
}

ImageInfiniteLight* CreateImageInfiniteLight(Scene& scene, std::string filename, const Transform& tf)
{
    return scene.CreateLight<ImageInfiniteLight>(scene.CreateImageTexture<Spectrum>(ReadImage3(filename, false)), tf);
}

} // namespace bulbit
