#pragma once

#include "material.h"

namespace spt
{

class Dielectric : public Material
{
public:
    Dielectric(double index_of_refraction);

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;

    double ior; // Index of Refraction

private:
    static double Reflectance(double cosine, double ref_idx);
};

inline Dielectric::Dielectric(double index_of_refraction)
    : ior{ index_of_refraction }
{
}

inline double Dielectric::Reflectance(double cosine, double ref_idx)
{
    // Schlick's approximation for reflectance.
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;

    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
}

} // namespace spt
