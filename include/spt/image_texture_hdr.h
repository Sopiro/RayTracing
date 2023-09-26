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

} // namespace spt
