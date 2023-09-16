#pragma once

#include "solid_color.h"
#include "texture.h"

namespace spt
{

class CheckerTexture : public Texture
{
public:
    CheckerTexture() = default;
    CheckerTexture(const Ref<Texture> even, const Ref<Texture> odd);
    CheckerTexture(Color color1, Color color2);

    virtual Color Value(const UV& uv, const Vec3& p) const override;

public:
    Ref<Texture> odd, even;
};

inline CheckerTexture::CheckerTexture(const Ref<Texture> _even, const Ref<Texture> _odd)
    : even{ _even }
    , odd{ _odd }
{
}

inline CheckerTexture::CheckerTexture(Color c1, Color c2)
    : even{ SolidColor::Create(c1) }
    , odd{ SolidColor::Create(c2) }
{
}

inline Color CheckerTexture::Value(const UV& uv, const Point3& p) const
{
    f64 sines = std::sin(10.0 * p.x) * std::sin(10.0 * p.y) * std::sin(10.0 * p.z);

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
