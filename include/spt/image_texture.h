#pragma once

#include <unordered_map>

#include "texture.h"

namespace spt
{

class ImageTexture : public Texture
{
public:
    inline static int32 texture_count = 0;
    inline static std::unordered_map<std::string, Ref<ImageTexture>> loaded_textures;
    static Ref<ImageTexture> Create(const std::string& path, bool srgb = false, bool hdr = false);

    virtual ~ImageTexture();

    ImageTexture(const ImageTexture&) = delete;
    ImageTexture& operator=(const ImageTexture&) = delete;

    virtual Color Value(const Point2& uv) const override;

protected:
    ImageTexture();
    ImageTexture(const std::string& path, bool srgb);

    const static int32 bytes_per_pixel = STBI_rgb;

    void* data;
    int32 width, height;
    int32 bytes_per_scanline;
};

class ImageTextureHDR : public ImageTexture
{
public:
    virtual ~ImageTextureHDR() = default;

    virtual Color Value(const Point2& uv) const override;

protected:
    friend class ImageTexture;

    ImageTextureHDR() = default;
    ImageTextureHDR(const std::string& path, bool srgb);
};

} // namespace spt
