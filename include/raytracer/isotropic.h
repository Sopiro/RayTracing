#pragma once

#include "material.h"

namespace spt
{

class Isotropic : public Material
{
public:
    Isotropic(Color c)
        : albedo(std::make_shared<SolidColor>(c))
    {
    }
    Isotropic(std::shared_ptr<Texture> a)
        : albedo(a)
    {
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;

public:
    std::shared_ptr<Texture> albedo;
};

inline bool Isotropic::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    out_srec.is_specular = true;
    out_srec.pdf = nullptr;
    out_srec.attenuation = albedo->Value(in_rec.uv, in_rec.point);
    out_srec.specular_ray = Ray{ in_rec.point, RandomInUnitSphere() };

    return true;
}

} // namespace spt
