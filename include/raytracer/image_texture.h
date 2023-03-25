#pragma once

#include <unordered_map>

#include "texture.h"

namespace spt
{

class ImageTexture : public Texture
{
public:
    static std::shared_ptr<ImageTexture> Create(std::string path, bool srgb = false);

    ~ImageTexture();

    ImageTexture(const ImageTexture&) = delete;
    ImageTexture& operator=(const ImageTexture&) = delete;

    ImageTexture(ImageTexture&&) noexcept = delete;
    ImageTexture& operator=(ImageTexture&&) noexcept = delete;

    virtual Color Value(const UV& uv, const Vec3& p) const override;

private:
    ImageTexture();
    ImageTexture(std::string path, bool srgb);

    const static int32 bytes_per_pixel = 3;

    uint8* data;
    int32 width, height;
    int32 bytes_per_scanline;
    std::string name;
};

inline ImageTexture::ImageTexture()
    : data{ nullptr }
    , width{ 0 }
    , height{ 0 }
    , bytes_per_scanline{ 0 }
{
}

inline ImageTexture::ImageTexture(std::string path, bool srgb)
{
    int32 components_per_pixel = bytes_per_pixel;

    data = stbi_load(path.data(), &width, &height, &components_per_pixel, components_per_pixel);

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
                data[i] = static_cast<uint8>(fmin(pow(data[i] / 255.0, 2.2) * 255.0, 255.0));
            }
        }
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

inline ImageTexture::~ImageTexture()
{
    stbi_image_free(data);
}

static int32 texture_count = 0;
static std::unordered_map<std::string, std::shared_ptr<ImageTexture>> loaded_textures;

inline std::shared_ptr<ImageTexture> ImageTexture::Create(std::string path, bool srgb)
{
    auto loaded = loaded_textures.find(path);
    if (loaded != loaded_textures.end())
    {
        return loaded->second;
    }

    std::shared_ptr<ImageTexture> image{ new ImageTexture(path, srgb) };
    loaded_textures.emplace(path, image);
    ++texture_count;

    return image;
}

inline Color ImageTexture::Value(const UV& uv, const Vec3& p) const
{
    if (data == nullptr)
    {
        return Color{ 1.0, 0.0, 1.0 };
    }

    // Clamp input texture coordinates to [0,1] x [1,0]
    double u = Clamp(uv.x, 0.0, 1.0);
    double v = 1.0 - Clamp(uv.y, 0.0, 1.0); // Flip V to image coordinates

    int32 i = static_cast<int32>(u * width);
    int32 j = static_cast<int32>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width)
    {
        i = width - 1;
    }
    if (j >= height)
    {
        j = height - 1;
    }

    double color_scale = 1.0 / 255.0;
    uint8* pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return Color{ color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2] };
}

} // namespace spt
