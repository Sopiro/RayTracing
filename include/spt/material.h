#pragma once

#include "common.h"
#include "pdf.h"
#include "ray.h"
#include "solid_color.h"
#include "texture.h"

namespace spt
{

struct Intersection;

struct Interaction
{
    bool is_specular;

    Ray specular_ray;
    Color attenuation;

    Ref<PDF> pdf; // Scattering pdf
};

// Lambertian
// Metal
// Dielectric
// Isotropic
// Microfacet
// DiffuseLight
class Material
{
public:
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const = 0;

    virtual Color Emit(const Intersection& is, const Vec3& wi) const;

    // BRDF with cosine term
    virtual Vec3 Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const;

    inline static Ref<Material> fallback = nullptr;
};

inline Color Material::Emit(const Intersection& is, const Vec3& wi) const
{
    return zero_vec3;
}

inline Vec3 Material::Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const
{
    assert(false);
    return zero_vec3;
}

} // namespace spt