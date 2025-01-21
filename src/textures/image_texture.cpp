#include "bulbit/textures.h"

namespace bulbit
{

FloatImageTexture* CreateFloatImageTexture(std::string filename, int32 channel, bool is_non_color)
{
    Image1 image = ReadImage1(filename, channel, is_non_color);
    if (image)
    {
        return texture_pool.pool_2d1f.Create({ filename, channel, is_non_color }, std::move(image), TexCoordFilter::repeat);
    }
    else
    {
        return nullptr;
    }
}

SpectrumImageTexture* CreateSpectrumImageTexture(std::string filename, bool is_non_color)
{
    Image3 image = ReadImage3(filename, is_non_color);
    if (image)
    {
        return texture_pool.pool_2d3f.Create({ filename, is_non_color }, std::move(image), TexCoordFilter::repeat);
    }
    else
    {
        return nullptr;
    }
}

} // namespace bulbit