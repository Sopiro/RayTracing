#include "bulbit/texture.h"

namespace bulbit
{

FloatCheckerTexture* CreateFloatCheckerTexture(const FloatTexture* a, const FloatTexture* b, const Point2& resolution)
{
    return texture_pool.pool_c1f.Create({ a, b, resolution }, a, b, resolution);
}

SpectrumCheckerTexture* CreateSpectrumCheckerTexture(const SpectrumTexture* a, const SpectrumTexture* b, const Point2& resolution)
{
    return texture_pool.pool_c3f.Create({ a, b, resolution }, a, b, resolution);
}

} // namespace bulbit
