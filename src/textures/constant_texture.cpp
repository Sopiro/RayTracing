#include "bulbit/textures.h"

namespace bulbit
{

FloatConstantTexture* CreateFloatConstantTexture(Float value)
{
    return texture_pool.GetPool0d<Float>().Create(value, value);
}

SpectrumConstantTexture* CreateSpectrumConstantTexture(const Spectrum& value)
{
    return texture_pool.GetPool0d<Spectrum>().Create(value, value);
}

SpectrumConstantTexture* CreateSpectrumConstantTexture(Float value)
{
    Spectrum sp{ value, value, value };
    return texture_pool.GetPool0d<Spectrum>().Create(sp, sp);
}

SpectrumConstantTexture* CreateSpectrumConstantTexture(Float r, Float g, Float b)
{
    Spectrum sp{ r, g, b };
    return texture_pool.GetPool0d<Spectrum>().Create(sp, sp);
}

} // namespace bulbit