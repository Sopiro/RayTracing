#pragma once

#include <unordered_map>

#include "texture.h"

namespace bulbit
{

enum TexCoordFilter
{
    repeat,
    clamp,
};

class ImageTexture : public Texture
{
public:
    inline static int32 texture_count = 0;
    inline static std::unordered_map<std::string, Ref<ImageTexture>> loaded_textures;
    static Ref<ImageTexture> Create(const std::string& filename, bool srgb = false);

    ImageTexture();
    ImageTexture(const std::string& filename, bool srgb);

    virtual Spectrum Evaluate(const Point2& uv) const override;

    int32 GetWidth() const;
    int32 GetHeight() const;

protected:
    void FilterTexCoord(int32* u, int32* v) const;

    std::unique_ptr<Spectrum[]> pixels;
    int32 width, height;
    TexCoordFilter texcoord_filter;
};

inline int32 ImageTexture::GetWidth() const
{
    return width;
}

inline int32 ImageTexture::GetHeight() const
{
    return height;
}

} // namespace bulbit
