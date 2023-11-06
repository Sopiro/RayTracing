#pragma once

#include "brdf.h"
#include "constant_color.h"
#include "cosine_pdf.h"
#include "ggx_pdf.h"
#include "ggxvndf_pdf.h"
#include "ray.h"

namespace bulbit
{

struct Intersection;

struct Interaction
{
    bool is_specular;

    Ray specular_ray;
    Spectrum attenuation;

    const BRDF* GetScatteringPDF() const
    {
        assert(is_specular == false);
        return (BRDF*)mem;
    }

private:
    friend class Lambertian;
    friend class Microfacet;

    inline static constexpr size_t pdf_mem_size =
        std::max(std::max(sizeof(LambertianReflection), sizeof(MicrofacetGGX)), sizeof(MicrofacetGGXVNDF));
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
    virtual bool IsLightSource() const;

    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const;

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const = 0;

    // BRDF + cosine term
    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const;

    inline static Ref<Material> fallback = nullptr;
};

inline bool Material::IsLightSource() const
{
    return false;
}

inline Spectrum Material::Emit(const Intersection& is, const Vec3& wi) const
{
    return RGBSpectrum::black;
}

inline Spectrum Material::Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const
{
    assert(false);
    return RGBSpectrum::black;
}

} // namespace bulbit