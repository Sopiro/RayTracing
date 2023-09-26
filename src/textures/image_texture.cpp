#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "spt/image_texture_hdr.h"

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

ImageTexture::ImageTexture(const std::string& path, bool srgb)
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

Color ImageTexture::Value(const UV& uv) const
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

    f64 color_scale = 1.0 / 255.0;
    u8* pixel = (u8*)(data) + j * bytes_per_scanline + i * bytes_per_pixel;

    return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}

} // namespace spt
