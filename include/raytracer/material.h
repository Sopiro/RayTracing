#pragma once

#include "common.h"
#include "pdf.h"
#include "ray.h"
#include "texture.h"

namespace spt
{

struct HitRecord;

struct ScatterRecord
{
    Ray specular_ray;
    bool is_specular;
    Color attenuation;
    std::shared_ptr<PDF> pdf;
};

class Material
{
public:
    virtual Color Emit(const Ray& in_ray, const HitRecord& in_rec) const
    {
        return Color{ 0.0, 0.0, 0.0 };
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const = 0;

    // BRDF with cosine term and without albedo
    virtual double ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
    {
        return 0.0;
    }
};

} // namespace spt