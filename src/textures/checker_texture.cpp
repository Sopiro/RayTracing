#include "bulbit/textures.h"

namespace bulbit
{

FloatCheckerTexture* CreateFloatCheckerTexture(const FloatTexture* a, const FloatTexture* b, const Point2& resolution)
{
    return texture_pool.GetPoolC<Float>().Create({ a, b, resolution }, a, b, resolution);
}

SpectrumCheckerTexture* CreateSpectrumCheckerTexture(const SpectrumTexture* a, const SpectrumTexture* b, const Point2& resolution)
{
    return texture_pool.GetPoolC<Spectrum>().Create({ a, b, resolution }, a, b, resolution);
}

} // namespace bulbit
