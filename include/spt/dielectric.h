#pragma once

#include "material.h"

namespace spt
{

class Dielectric : public Material
{
public:
    Dielectric(float64 index_of_refraction);

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;

    float64 ior; // Index of Refraction

private:
    static float64 Reflectance(float64 cosine, float64 ref_idx);
};

inline Dielectric::Dielectric(float64 index_of_refraction)
    : ior{ index_of_refraction }
{
}

inline float64 Dielectric::Reflectance(float64 cosine, float64 ref_idx)
{
    // Schlick's approximation for reflectance.
    float64 r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;

    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
}

} // namespace spt
