#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "spt/image_texture.h"

namespace spt
{

Ref<ImageTexture> ImageTexture::Create(const std::string& path, bool srgb)
{
    auto loaded = loaded_textures.find(path);
    if (loaded != loaded_textures.end())
    {
        return loaded->second;
    }

    auto image = Ref<ImageTexture>(new ImageTexture(path, srgb));

    loaded_textures.emplace(path, image);
    ++texture_count;

    return image;
}

ImageTexture::ImageTexture()
    : pixels{ nullptr }
    , width{ 0 }
    , height{ 0 }
{
}

ImageTexture::ImageTexture(const std::string& path, bool srgb)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_scale(1.0f);
    stbi_ldr_to_hdr_gamma(srgb ? 2.2f : 1.0f);

    int32 components_per_pixel;
    float* data = stbi_loadf(path.data(), &width, &height, &components_per_pixel, STBI_rgb);

    if (!data)
    {
        std::cerr << "ERROR: Could not load texture file '" << path << std::endl;
        width = 0;
        height = 0;
        return;
    }

    pixels = std::make_unique<Spectrum[]>(width * height);

#pragma omp parallel for
    for (int32 i = 0; i < width * height; ++i)
    {
        pixels[i].r = std::fmax(0, data[components_per_pixel * i + 0]);
        pixels[i].g = std::fmax(0, data[components_per_pixel * i + 1]);
        pixels[i].b = std::fmax(0, data[components_per_pixel * i + 2]);
    }

    stbi_image_free(data);
}

Spectrum ImageTexture::Value(const Point2& uv) const
{
    int32 i, j;
    UVtoIndices(&i, &j, uv, width, height);

    Spectrum pixel = pixels[i + j * width];
    return pixel;
}

} // namespace spt
