#pragma once

#include "image_texture.h"

namespace spt
{

class ImageTextureHDR : public ImageTexture
{
public:
    virtual ~ImageTextureHDR() = default;

    virtual Color Value(const UV& uv) const override;

protected:
    friend class ImageTexture;

    ImageTextureHDR() = default;
    ImageTextureHDR(const std::string& path, bool srgb);
};

inline ImageTextureHDR::ImageTextureHDR(const std::string& path, bool srgb)
{
    i32 components_per_pixel;
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
        for (i32 i = 0; i < width * height * bytes_per_pixel; ++i)
        {
            // Clamp needed
            f32 value = *((f32*)data + i);
            *((f32*)data + i) = Clamp(value, 0.0f, 10.0f);
        }
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

inline Color ImageTextureHDR::Value(const UV& uv) const
{
    f64 u = fmod(uv.x, 1.0);
    f64 v = fmod(uv.y, 1.0);

    if (u < 0.0) ++u;
    if (v < 0.0) ++v;

    // Flip V to image coordinates
    v = 1.0 - v;

    i32 i = i32(u * width);
    i32 j = i32(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width) i = width - 1;
    if (j >= height) j = height - 1;

    f32* pixel = (f32*)data + j * bytes_per_scanline + i * bytes_per_pixel;

    return Color(pixel[0], pixel[1], pixel[2]);
}

} // namespace spt
