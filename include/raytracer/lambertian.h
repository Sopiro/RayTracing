#pragma once

#include "material.h"

namespace spt
{

class Lambertian : public Material
{
public:
    Lambertian(const Color& color);
    Lambertian(std::shared_ptr<Texture> albedo);

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;
    virtual double ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const override;

public:
    std::shared_ptr<Texture> albedo;
};

inline Lambertian::Lambertian(const Color& _color)
    : albedo{ SolidColor::Create(_color) }
{
}
inline Lambertian::Lambertian(std::shared_ptr<Texture> _albedo)
    : albedo{ _albedo }
{
}

inline bool Lambertian::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    out_srec.is_specular = false;
    out_srec.attenuation = albedo->Value(in_rec.uv, in_rec.point);
    out_srec.pdf = std::make_shared<CosinePDF>(in_rec.normal);

    return true;
}

inline double Lambertian::ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
{
    // Cosine density
    return Dot(in_rec.normal, in_scattered.dir) / pi;
}

} // namespace spt
