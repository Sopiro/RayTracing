#pragma once

#include "bulbit/scene.h"
#include "bulbit/textures.h"

namespace bulbit
{

inline ConstantTexture<Float>* CreateFloatConstantTexture(Scene& scene, Float value)
{
    return scene.CreateTexture<ConstantTexture, Float>(value);
}

inline ConstantTexture<Spectrum>* CreateSpectrumConstantTexture(Scene& scene, const Spectrum& value)
{
    return scene.CreateTexture<ConstantTexture, Spectrum>(value);
}

inline ConstantTexture<Spectrum>* CreateSpectrumConstantTexture(Scene& scene, Float value)
{
    return scene.CreateTexture<ConstantTexture, Spectrum>(Spectrum{ value });
}

inline ConstantTexture<Spectrum>* CreateSpectrumConstantTexture(Scene& scene, Float r, Float g, Float b)
{
    return scene.CreateTexture<ConstantTexture, Spectrum>(Spectrum{ r, g, b });
}

inline ImageTexture<Float>* CreateFloatImageTexture(Scene& scene, std::string filename, int32 channel, bool non_color = false)
{
    return scene.CreateTexture<ImageTexture, Float>(ReadImage1(filename, channel, non_color));
}

inline ImageTexture<Spectrum>* CreateSpectrumImageTexture(Scene& scene, std::string filename, bool non_color = false)
{
    return scene.CreateTexture<ImageTexture, Spectrum>(ReadImage3(filename, non_color));
}

inline CheckerTexture<Float>* CreateFloatCheckerTexture(
    Scene& scene, Texture<Float>* a, Texture<Float>* b, const Point2& resolution
)
{
    return scene.CreateTexture<CheckerTexture, Float>(a, b, resolution);
}

inline CheckerTexture<Spectrum>* CreateSpectrumCheckerTexture(
    Scene& scene, Texture<Spectrum>* a, Texture<Spectrum>* b, const Point2& resolution
)
{
    return scene.CreateTexture<CheckerTexture, Spectrum>(a, b, resolution);
}

} // namespace bulbit
