#pragma once

#include "common.h"
#include "pdf.h"
#include "ray.h"
#include "solid_color.h"
#include "texture.h"

namespace spt
{

struct HitRecord;

struct ScatterRecord
{
    bool is_specular;
    Ray specular_ray;
    Color attenuation;
    Ref<PDF> pdf; // Scattering pdf
};

class Material
{
public:
    virtual Color Emit(const Ray& in_ray, const HitRecord& in_rec) const
    {
        return Color{ 0.0, 0.0, 0.0 };
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const = 0;

    // BRDF with cosine term
    virtual Vec3 Evaluate(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
    {
        assert(false);
        return Vec3{ 0.0 };
    }

    inline static Ref<Material> fallback_material = nullptr;
};

} // namespace spt