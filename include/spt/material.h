#pragma once

#include "cosine_pdf.h"
#include "ggx_pdf.h"
#include "ggxvndf_pdf.h"
#include "pdf.h"

#include "constant_color.h"
#include "ray.h"

namespace spt
{

struct Intersection;

struct Interaction
{
    bool is_specular;

    Ray specular_ray;
    Spectrum attenuation;

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
    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const;

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const = 0;

    // BRDF + cosine term
    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const;

    inline static Ref<Material> fallback = nullptr;
};

inline Spectrum Material::Emit(const Intersection& is, const Vec3& wi) const
{
    return RGBSpectrum::black;
}

inline Spectrum Material::Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const
{
    assert(false);
    return RGBSpectrum::black;
}

} // namespace spt