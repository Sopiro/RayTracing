#pragma once

#include "bulbit/scene.h"
#include "bulbit/textures.h"

namespace bulbit
{

ConstantTexture<Float>* CreateFloatConstantTexture(Scene& scene, Float value);
ConstantTexture<Spectrum>* CreateSpectrumConstantTexture(Scene& scene, const Spectrum& value);
ConstantTexture<Spectrum>* CreateSpectrumConstantTexture(Scene& scene, Float value);
ConstantTexture<Spectrum>* CreateSpectrumConstantTexture(Scene& scene, Float r, Float g, Float b);
ImageTexture<Float>* CreateFloatImageTexture(Scene& scene, std::string filename, int32 channel, bool non_color = false);
ImageTexture<Spectrum>* CreateSpectrumImageTexture(Scene& scene, std::string filename, bool non_color = false);
CheckerTexture<Float>* CreateFloatCheckerTexture(Scene& scene, Float a, Float b, const Point2& resolution);
CheckerTexture<Float>* CreateFloatCheckerTexture(
    Scene& scene, const Texture<Float>* a, const Texture<Float>* b, const Point2& resolution
);
CheckerTexture<Spectrum>* CreateSpectrumCheckerTexture(Scene& scene, Float a, Float b, const Point2& resolution);
CheckerTexture<Spectrum>* CreateSpectrumCheckerTexture(
    Scene& scene, const Spectrum& a, const Spectrum& b, const Point2& resolution
);
CheckerTexture<Spectrum>* CreateSpectrumCheckerTexture(
    Scene& scene, const Texture<Spectrum>* a, const Texture<Spectrum>* b, const Point2& resolution
);

} // namespace bulbit
