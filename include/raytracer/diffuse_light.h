#include "material.h"

namespace spt
{

class DiffuseLight : public Material
{
public:
    DiffuseLight(std::shared_ptr<Texture> a)
        : emit(a)
    {
    }

    DiffuseLight(Color c)
        : emit(std::make_shared<SolidColor>(c))
    {
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override
    {
        return false;
    }

    virtual Color Emit(const Ray& in_ray, const HitRecord& in_rec) const override;

public:
    std::shared_ptr<Texture> emit;
};

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
