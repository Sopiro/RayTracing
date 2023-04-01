#pragma once

#include "solid_color.h"
#include "texture.h"

namespace spt
{

class CheckerTexture : public Texture
{
public:
    CheckerTexture() = default;
    CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd);
    CheckerTexture(Color color1, Color color2);

    virtual Color Value(const UV& uv, const Vec3& p) const override;

public:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

inline CheckerTexture::CheckerTexture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd)
    : even{ _even }
    , odd{ _odd }
{
}

inline CheckerTexture::CheckerTexture(Color c1, Color c2)
    : even{ SolidColor::Create(c1) }
    , odd{ SolidColor::Create(c2) }
{
}

inline Color CheckerTexture::Value(const UV& uv, const Vec3& p) const
{
    double sines = sin(10.0 * p.x) * sin(10.0 * p.y) * sin(10.0 * p.z);

    if (sines < 0.0)
    {
        return odd->Value(uv, p);
    }
    else
    {
        return even->Value(uv, p);
    }
}

} // namespace spt
