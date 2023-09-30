#include "material.h"

namespace spt
{

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Ref<Texture> emission);
    DiffuseLight(Color color);

    virtual Color Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const override;

    Ref<Texture> emission;
    bool two_sided;
};

inline DiffuseLight::DiffuseLight(const Ref<Texture> _emission)
    : emission{ _emission }
    , two_sided{ false }
{
}

inline DiffuseLight::DiffuseLight(Color color)
    : emission{ ConstantColor::Create(color) }
{
}

inline Color DiffuseLight::Emit(const Intersection& is, const Vec3& wi) const
{
    if (is.front_face || two_sided)
    {
        return emission->Value(is.uv);
    }
    else
    {
        return zero_vec3;
    }
}

inline bool DiffuseLight::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi) const
{
    return false;
}

} // namespace spt
