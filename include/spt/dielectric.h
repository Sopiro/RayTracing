#pragma once

#include "material.h"

namespace spt
{

class Dielectric : public Material
{
public:
    Dielectric(f64 index_of_refraction);

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;

    f64 ior; // Index of Refraction

private:
    static f64 Reflectance(f64 cosine, f64 ref_idx);
};

inline Dielectric::Dielectric(f64 index_of_refraction)
    : ior{ index_of_refraction }
{
}

inline f64 Dielectric::Reflectance(f64 cosine, f64 ref_idx)
{
    // Schlick's approximation for reflectance.
    f64 r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;

    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
}

} // namespace spt
