#include "material.h"

namespace spt
{

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Ref<Texture>& emission);
    DiffuseLight(Color color);

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;
    virtual Color Emit(const Ray& in_ray, const HitRecord& in_rec) const override;

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

inline bool DiffuseLight::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    return false;
}

inline Color DiffuseLight::Emit(const Ray& in_ray, const HitRecord& in_rec) const
{
    if (in_rec.front_face || two_sided)
    {
        return emit->Value(in_rec.uv, in_rec.point);
    }
    else
    {
        return Color{ 0.0, 0.0, 0.0 };
    }
}

} // namespace spt
