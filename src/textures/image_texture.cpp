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
    : data{ nullptr }
    , width{ 0 }
    , height{ 0 }
    , bytes_per_scanline{ 0 }
{
}

ImageTexture::~ImageTexture()
{
    stbi_image_free(data);
}

ImageTexture::ImageTexture(const std::string& path, bool srgb)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_scale(1.0f);
    stbi_ldr_to_hdr_gamma(srgb ? 2.2f : 1.0f);

    int32 components_per_pixel;
    data = stbi_loadf(path.data(), &width, &height, &components_per_pixel, bytes_per_pixel);

    if (!data)
    {
        std::cerr << "ERROR: Could not load texture image file '" << path << "'.\n";
        width = 0;
        height = 0;
    }
    else
    {
#pragma omp parallel for
        for (int32 i = 0; i < width * height * bytes_per_pixel; ++i)
        {
            // Clamp needed
            float value = *((float*)data + i);
            *((float*)data + i) = std::fmax(0.0f, value);
            // *((float*)data + i) = Clamp(value, 0.0f, 100.0f);
        }
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

Spectrum ImageTexture::Value(const Point2& uv) const
{
    int32 i, j;
    UVtoIndices(&i, &j, uv, width, height);

    float* pixel = (float*)data + j * bytes_per_scanline + i * bytes_per_pixel;

    return Spectrum(pixel[0], pixel[1], pixel[2]);
}

} // namespace spt
