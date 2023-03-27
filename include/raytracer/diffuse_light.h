#include "material.h"

namespace spt
{

class DiffuseLight : public Material
{
public:
    DiffuseLight(std::shared_ptr<Texture> emission);
    DiffuseLight(Color color);

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;
    virtual Color Emit(const Ray& in_ray, const HitRecord& in_rec) const override;

public:
    std::shared_ptr<Texture> emit;
};

inline DiffuseLight::DiffuseLight(std::shared_ptr<Texture> emission)
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
    if (in_rec.front_face)
    {
        return emit->Value(in_rec.uv, in_rec.point);
    }
    else
    {
        return Color{ 0.0, 0.0, 0.0 };
    }
}

} // namespace spt
