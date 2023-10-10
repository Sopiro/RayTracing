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
    static Ref<ImageTexture> Create(const std::string& path, bool srgb = false);

    virtual ~ImageTexture();

    ImageTexture(const ImageTexture&) = delete;
    ImageTexture& operator=(const ImageTexture&) = delete;

    virtual Spectrum Value(const Point2& uv) const override;

    int32 GetWidth() const;
    int32 GetHeight() const;

protected:
    ImageTexture();
    ImageTexture(const std::string& path, bool srgb);

    static const int32 bytes_per_pixel = STBI_rgb;
    static void UVtoIndices(int32* i, int32* j, const Point2& uv, int32 w, int32 h);

    void* data;
    int32 width, height;
    int32 bytes_per_scanline;
};

inline int32 ImageTexture::GetWidth() const
{
    return width;
}

inline int32 ImageTexture::GetHeight() const
{
    return height;
}

inline void ImageTexture::UVtoIndices(int32* i, int32* j, const Point2& uv, int32 w, int32 h)
{
    // Wrap method
    Float u = std::fmod(uv.x, Float(1.0));
    Float v = std::fmod(uv.y, Float(1.0));

    if (u < 0) ++u;
    if (v < 0) ++v;

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    *i = std::min(int32(u * w), w - 1);
    *j = std::min(int32(v * h), h - 1);
}

} // namespace spt
