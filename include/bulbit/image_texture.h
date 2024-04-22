#pragma once

#include <unordered_map>

#include "pool.h"
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
    static ImageTexture* Create(const std::string& filename, bool srgb = false);

    ImageTexture();
    ImageTexture(const std::string& filename, bool srgb);

    virtual Spectrum Evaluate(const Point2& uv) const override;
    virtual Float EvaluateAlpha(const Point2& uv) const override;

    int32 GetWidth() const;
    int32 GetHeight() const;

protected:
    void FilterTexCoord(int32* u, int32* v) const;

    std::unique_ptr<RGBSpectrum[]> rgb;
    std::unique_ptr<Float[]> alpha;

    int32 width, height;
    TexCoordFilter texcoord_filter;

private:
    static inline Pool<std::string, ImageTexture> pool;
};

inline ImageTexture* ImageTexture::Create(const std::string& filename, bool srgb)
{
    return pool.Create(filename, srgb);
}

inline int32 ImageTexture::GetWidth() const
{
    return width;
}

inline int32 ImageTexture::GetHeight() const
{
    return height;
}

} // namespace bulbit
