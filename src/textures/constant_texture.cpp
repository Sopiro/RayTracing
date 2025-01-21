#include "bulbit/textures.h"

namespace bulbit
{

FloatConstantTexture* CreateFloatConstantTexture(Float value)
{
    return texture_pool.pool_1f.Create(value, value);
}

SpectrumConstantTexture* CreateSpectrumConstantTexture(const Spectrum& value)
{
    return texture_pool.pool_3f.Create(value, value);
}

SpectrumConstantTexture* CreateSpectrumConstantTexture(Float value)
{
    Spectrum sp{ value, value, value };
    return texture_pool.pool_3f.Create(sp, sp);
}

SpectrumConstantTexture* CreateSpectrumConstantTexture(Float r, Float g, Float b)
{
    Spectrum sp{ r, g, b };
    return texture_pool.pool_3f.Create(sp, sp);
}

} // namespace bulbit