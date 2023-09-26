#pragma once

#include "cosine_pdf.h"
#include "ggx_pdf.h"
#include "ggxvndf_pdf.h"
#include "pdf.h"

#include "ray.h"
#include "solid_color.h"

namespace spt
{

struct Intersection;

struct Interaction
{
    bool is_specular;

    Ray specular_ray;
    Color attenuation;

    const PDF* GetScatteringPDF() const
    {
        assert(is_specular == false);
        return (PDF*)mem;
    }

private:
    friend class Lambertian;
    friend class Microfacet;

    inline static constexpr size_t pdf_mem_size = std::max(std::max(sizeof(CosinePDF), sizeof(GGXPDF)), sizeof(GGXVNDFPDF));
    char mem[pdf_mem_size];
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
    virtual Color Emit(const Intersection& is, const Vec3& wi) const;

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const = 0;

    // BRDF + cosine term
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