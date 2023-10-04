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

    static const int32 bytes_per_pixel = STBI_rgb;
    static void UVtoIndices(int32* i, int32* j, const Point2& uv, int32 w, int32 h);

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

inline void ImageTexture::UVtoIndices(int32* i, int32* j, const Point2& uv, int32 w, int32 h)
{
    Float u = std::fmod(uv.x, Float(1.0));
    Float v = std::fmod(uv.y, Float(1.0));

    if (u < 0) ++u;
    if (v < 0) ++v;

    *i = int32(u * w);
    *j = int32(v * h);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (*i >= w) *i = w - 1;
    if (*j >= h) *j = h - 1;
}

} // namespace spt
