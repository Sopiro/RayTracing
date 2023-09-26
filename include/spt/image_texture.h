#pragma once

#include <unordered_map>

#include "texture.h"

namespace spt
{

class ImageTexture : public Texture
{
public:
    inline static i32 texture_count = 0;
    inline static std::unordered_map<std::string, Ref<ImageTexture>> loaded_textures;
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

inline ImageTexture::~ImageTexture()
{
    stbi_image_free(data);
}

} // namespace spt
