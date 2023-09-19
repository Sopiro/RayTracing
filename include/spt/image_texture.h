#pragma once

#include <unordered_map>

#include "texture.h"

namespace spt
{

class ImageTexture : public Texture
{
public:
    static Ref<ImageTexture> Create(const std::string& path, bool srgb = false, bool hdr = false);

    virtual ~ImageTexture();

    ImageTexture(const ImageTexture&) = delete;
    ImageTexture& operator=(const ImageTexture&) = delete;

    virtual Color Value(const UV& uv) const override;

protected:
    ImageTexture();
    ImageTexture(const std::string& path, bool srgb);

    const static i32 bytes_per_pixel = STBI_rgb;

    void* data;
    i32 width, height;
    i32 bytes_per_scanline;
};

inline ImageTexture::ImageTexture()
    : data{ nullptr }
    , width{ 0 }
    , height{ 0 }
    , bytes_per_scanline{ 0 }
{
}

inline ImageTexture::ImageTexture(const std::string& path, bool srgb)
{
    i32 components_per_pixel;
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
            for (i32 i = 0; i < width * height * bytes_per_pixel; ++i)
            {
                // Convert to linear space
                u8 value = *((u8*)data + i);
                *((u8*)data + i) = static_cast<u8>(std::fmin(std::pow(value / 255.0, 2.2) * 255.0, 255.0));
            }
        }
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

inline ImageTexture::~ImageTexture()
{
    stbi_image_free(data);
}

static i32 texture_count = 0;
static std::unordered_map<std::string, Ref<ImageTexture>> loaded_textures;

inline Color ImageTexture::Value(const UV& uv) const
{
    f64 u = fmod(uv.x, 1.0);
    f64 v = fmod(uv.y, 1.0);

    if (u < 0.0) ++u;
    if (v < 0.0) ++v;

    // Flip V to image coordinates
    v = 1.0 - v;

    i32 i = static_cast<i32>(u * width);
    i32 j = static_cast<i32>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width) i = width - 1;
    if (j >= height) j = height - 1;

    f64 color_scale = 1.0 / 255.0;
    u8* pixel = (u8*)(data) + j * bytes_per_scanline + i * bytes_per_pixel;

    return Color{ color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2] };
}

} // namespace spt
