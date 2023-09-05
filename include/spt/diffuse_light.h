#include "material.h"

namespace spt
{

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Ref<Texture>& emission);
    DiffuseLight(Color color);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Ray& wi) const override;
    virtual Color Emit(const Intersection& is, const Ray& wi) const override;

public:
    bool two_sided = false;
    Ref<Texture> emit;
};

inline DiffuseLight::DiffuseLight(const Ref<Texture>& emission)
    : emit{ emission }
{
}

inline DiffuseLight::DiffuseLight(Color color)
    : emit{ SolidColor::Create(color) }
{
}

inline bool DiffuseLight::Scatter(Interaction* ir, const Intersection& is, const Ray& wi) const
{
    return false;
}

inline Color DiffuseLight::Emit(const Intersection& is, const Ray& wi) const
{
    if (is.front_face || two_sided)
    {
        return emit->Value(is.uv, is.point);
    }
    else
    {
        return Color{ 0.0, 0.0, 0.0 };
    }
}

} // namespace spt
