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

class Material
{
public:
    virtual Color Emit(const Ray& in_wi, const Intersection& in_is) const
    {
        return Color{ 0.0, 0.0, 0.0 };
    }

    virtual bool Scatter(const Ray& in_wi, const Intersection& in_is, Interaction& out_ir) const = 0;

    // BRDF with cosine term
    virtual Vec3 Evaluate(const Ray& in_wi, const Intersection& in_is, const Ray& in_wo) const
    {
        assert(false);
        return Vec3{ 0.0 };
    }

    inline static Ref<Material> fallback_material = nullptr;
};

} // namespace spt