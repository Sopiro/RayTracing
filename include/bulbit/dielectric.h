#pragma once

#include "material.h"

namespace bulbit
{

class Dielectric : public Material
{
public:
    Dielectric(Float index_of_refraction);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;

    Float ior; // Index of Refraction

private:
    static Float Reflectance(Float cosine, Float ref_idx);
};

inline Float Dielectric::Reflectance(Float cosine, Float ref_idx)
{
    // Schlick's approximation for reflectance.
    Float r0 = Sqr((1 - ref_idx) / (1 + ref_idx));

    return r0 + (1 - r0) * std::pow((1 - cosine), Float(5.0));
}

} // namespace bulbit
