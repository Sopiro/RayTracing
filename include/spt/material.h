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
    virtual Color Emit(const Intersection& is, const Ray& wi) const
    {
        return Color{ 0.0, 0.0, 0.0 };
    }

    virtual bool Scatter(const Intersection& is, const Ray& wi, Interaction& out_ir) const = 0;

    // BRDF with cosine term
    virtual Vec3 Evaluate(const Intersection& is, const Ray& wi, const Ray& wo) const
    {
        assert(false);
        return Vec3{ 0.0 };
    }

    inline static Ref<Material> fallback_material = nullptr;
};

} // namespace spt