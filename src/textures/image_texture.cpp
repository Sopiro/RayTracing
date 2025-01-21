#include "bulbit/textures.h"

namespace bulbit
{

FloatImageTexture* CreateFloatImageTexture(std::string filename, int32 channel, bool is_non_color)
{
    return texture_pool.pool_2d1f.Create(
        { filename, channel, is_non_color }, ReadImage1(filename, channel, is_non_color), TexCoordFilter::repeat
    );
}

SpectrumImageTexture* CreateSpectrumImageTexture(std::string filename, bool is_non_color)
{
    return texture_pool.pool_2d3f.Create({ filename, is_non_color }, ReadImage3(filename, is_non_color), TexCoordFilter::repeat);
}

} // namespace bulbit