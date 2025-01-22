#include "bulbit/textures.h"

namespace bulbit
{

FloatImageTexture* CreateFloatImageTexture(Image1 image)
{
    if (image)
    {
        return texture_pool.GetPool2d<Float>().Create(
            { &image[0], image.width * image.height }, std::move(image), TexCoordFilter::repeat
        );
    }
    else
    {
        return nullptr;
    }
}

SpectrumImageTexture* CreateSpectrumImageTexture(Image3 image)
{
    if (image)
    {
        return texture_pool.GetPool2d<Spectrum>().Create(
            { &image[0], image.width * image.height }, std::move(image), TexCoordFilter::repeat
        );
    }
    else
    {
        return nullptr;
    }
}

} // namespace bulbit