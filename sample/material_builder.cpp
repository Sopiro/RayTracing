#include "material_builder.h"
#include "texture_builder.h"

namespace bulbit
{

DiffuseMaterial* CreateDiffuseMaterial(Scene& scene, Float reflectance, const SpectrumTexture* normalmap, Float alpha)
{
    return scene.CreateMaterial<DiffuseMaterial>(
        CreateSpectrumConstantTexture(scene, reflectance), normalmap, CreateFloatConstantTexture(scene, alpha)
    );
}
DiffuseMaterial* CreateDiffuseMaterial(Scene& scene, const Spectrum& reflectance, const SpectrumTexture* normalmap, Float alpha)
{
    return scene.CreateMaterial<DiffuseMaterial>(
        CreateSpectrumConstantTexture(scene, reflectance), normalmap, CreateFloatConstantTexture(scene, alpha)
    );
}

DiffuseMaterial* CreateDiffuseMaterial(Scene& scene, const std::string& filename, const SpectrumTexture* normalmap, Float alpha)
{
    return scene.CreateMaterial<DiffuseMaterial>(
        CreateSpectrumImageTexture(scene, filename), normalmap, CreateFloatConstantTexture(scene, alpha)
    );
}

DielectricMaterial* CreateDielectricMaterial(Scene& scene, Float eta, Float roughness, const SpectrumTexture* normalmap)
{
    return scene.CreateMaterial<DielectricMaterial>(
        eta, CreateFloatConstantTexture(scene, roughness), CreateFloatConstantTexture(scene, roughness), normalmap
    );
}

ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& eta, const Spectrum& k, Float roughness, const SpectrumTexture* normalmap, Float alpha
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        CreateSpectrumConstantTexture(scene, eta), CreateSpectrumConstantTexture(scene, k),
        CreateFloatConstantTexture(scene, roughness), CreateFloatConstantTexture(scene, roughness), normalmap,
        CreateFloatConstantTexture(scene, alpha)
    );
}

ConductorMaterial* CreateConductorMaterial(
    Scene& scene,
    const Spectrum& eta,
    const Spectrum& k,
    Float roughness_u,
    Float roughness_v,
    const SpectrumTexture* normalmap,
    Float alpha
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        CreateSpectrumConstantTexture(scene, eta), CreateSpectrumConstantTexture(scene, k),
        CreateFloatConstantTexture(scene, roughness_u), CreateFloatConstantTexture(scene, roughness_v), normalmap,
        CreateFloatConstantTexture(scene, alpha)
    );
}

ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& reflectance, Float roughness, const SpectrumTexture* normalmap, Float alpha
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        CreateSpectrumConstantTexture(scene, reflectance), CreateFloatConstantTexture(scene, roughness),
        CreateFloatConstantTexture(scene, roughness), normalmap, CreateFloatConstantTexture(scene, alpha)
    );
}

ConductorMaterial* CreateConductorMaterial(
    Scene& scene, const Spectrum& reflectance, Float roughness_u, Float roughness_v, const SpectrumTexture* normalmap, Float alpha
)
{
    return scene.CreateMaterial<ConductorMaterial>(
        CreateSpectrumConstantTexture(scene, reflectance), CreateFloatConstantTexture(scene, roughness_u),
        CreateFloatConstantTexture(scene, roughness_v), normalmap, CreateFloatConstantTexture(scene, alpha)
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
        CreateSpectrumConstantTexture(scene, basecolor), CreateFloatConstantTexture(scene, metallic),
        CreateFloatConstantTexture(scene, roughness), CreateFloatConstantTexture(scene, roughness),
        CreateSpectrumConstantTexture(scene, emission), normalmap, alpha
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
        CreateSpectrumConstantTexture(scene, basecolor), CreateFloatConstantTexture(scene, metallic),
        CreateFloatConstantTexture(scene, u_roughness), CreateFloatConstantTexture(scene, v_roughness),
        CreateSpectrumConstantTexture(scene, emission), normalmap, alpha
    );
}

SubsurfaceDiffusionMaterial* CreateSubsurfaceDiffusionMaterial(
    Scene& scene, const Spectrum& reflectance, const Spectrum& mfp, Float eta, Float roughness, const SpectrumTexture* normalmap
)
{
    return scene.CreateMaterial<SubsurfaceDiffusionMaterial>(
        CreateSpectrumConstantTexture(scene, reflectance), mfp, eta, CreateFloatConstantTexture(scene, roughness),
        CreateFloatConstantTexture(scene, roughness), normalmap
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
        reflectance, mfp, eta, CreateFloatConstantTexture(scene, roughness), CreateFloatConstantTexture(scene, roughness),
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
        reflectance, mfp, eta, CreateFloatConstantTexture(scene, roughness), CreateFloatConstantTexture(scene, roughness), g,
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
        CreateSpectrumConstantTexture(scene, reflectance), mfp, eta, CreateFloatConstantTexture(scene, roughness),
        CreateFloatConstantTexture(scene, roughness), g, normalmap
    );
}

MixtureMaterial* CreateMixtureMaterial(Scene& scene, const Material* material1, const Material* material2, Float amount)
{
    return scene.CreateMaterial<MixtureMaterial>(material1, material2, CreateFloatConstantTexture(scene, amount));
}

MirrorMaterial* CreateMirrorMaterial(Scene& scene, const Spectrum& reflectance, const SpectrumTexture* normalmap, Float alpha)
{
    return scene.CreateMaterial<MirrorMaterial>(
        CreateSpectrumConstantTexture(scene, reflectance), normalmap, CreateFloatConstantTexture(scene, alpha)
    );
}

DiffuseLightMaterial* CreateDiffuseLightMaterial(Scene& scene, Float emission, bool two_sided, Float alpha)
{
    return scene.CreateMaterial<DiffuseLightMaterial>(
        CreateSpectrumConstantTexture(scene, emission), two_sided, CreateFloatConstantTexture(scene, alpha)
    );
}

DiffuseLightMaterial* CreateDiffuseLightMaterial(Scene& scene, const Spectrum& emission, bool two_sided, Float alpha)
{
    return scene.CreateMaterial<DiffuseLightMaterial>(
        CreateSpectrumConstantTexture(scene, emission), two_sided, CreateFloatConstantTexture(scene, alpha)
    );
}

const Material* CreateRandomUnrealMaterial(Scene& scene)
{
    // clang-format off
    Spectrum basecolor = Spectrum(Rand(0.0f, 1.0f), Rand(0.0f, 1.0f), Rand(0.0f, 1.0f)) * 0.7f;
    return CreateUnrealMaterial(scene, 
        basecolor,
        Rand() > 0.5f ? 1.0f : 0.0f,
        (Float)std::sqrt(Rand(0.1f, 1.0f)),
        (Float)std::sqrt(Rand(0.1f, 1.0f)),
        basecolor * (Rand() < 0.08f ? Rand(0.0f, 0.3f) : 0.0f)
    );
    // clang-format on
}

} // namespace bulbit
