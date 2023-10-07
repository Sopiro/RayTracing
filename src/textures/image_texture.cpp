#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "spt/image_texture.h"

namespace spt
{

Ref<ImageTexture> ImageTexture::Create(const std::string& path, bool srgb, bool hdr)
{
    auto loaded = loaded_textures.find(path);
    if (loaded != loaded_textures.end())
    {
        return loaded->second;
    }

    Ref<ImageTexture> image =
        hdr ? Ref<ImageTextureHDR>(new ImageTextureHDR(path, srgb)) : Ref<ImageTexture>(new ImageTexture(path, srgb));

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

    int32 components_per_pixel;
    data = stbi_load(path.data(), &width, &height, &components_per_pixel, bytes_per_pixel);

    if (!data)
    {
        std::cerr << "ERROR: Could not load texture image file '" << path << "'.\n";
        width = 0;
        height = 0;
    }
    else
    {
        if (srgb)
        {
#pragma omp parallel for
            for (int32 i = 0; i < width * height * bytes_per_pixel; ++i)
            {
                // Convert to linear space
                uint8 value = *((uint8*)data + i);
                *((uint8*)data + i) = uint8(std::fmin(std::pow(value / 255.0, 2.2) * 255.0, 255.0));
            }
        }
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

Color ImageTexture::Value(const Point2& uv) const
{
    int32 i, j;
    UVtoIndices(&i, &j, uv, width, height);

    constexpr Float color_scale = 1 / Float(255.0);
    uint8* pixel = (uint8*)(data) + j * bytes_per_scanline + i * bytes_per_pixel;

    return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}

ImageTextureHDR::ImageTextureHDR(const std::string& path, bool srgb)
{
    stbi_set_flip_vertically_on_load(true);

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

Color ImageTextureHDR::Value(const Point2& uv) const
{
    int32 i, j;
    UVtoIndices(&i, &j, uv, width, height);

    float* pixel = (float*)data + j * bytes_per_scanline + i * bytes_per_pixel;

    return Color(pixel[0], pixel[1], pixel[2]);
}

} // namespace spt
