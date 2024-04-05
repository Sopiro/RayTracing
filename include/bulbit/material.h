#pragma once

#include "constant_color.h"
#include "pdf.h"
#include "ray.h"

namespace bulbit
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

    inline static constexpr size_t pdf_mem_size =
        std::max(std::max(sizeof(LambertianReflection), sizeof(MicrofacetGGX)), sizeof(MicrofacetGGXVNDF));
    char mem[pdf_mem_size];
};

// Lambertian
// Microfacet
// Metal
// Dielectric
// DiffuseLight
class Material
{
public:
    virtual bool IsLightSource() const
    {
        return false;
    }

    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const
    {
        return RGBSpectrum::black;
    }

    // BRDF * cosine term
    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const
    {
        assert(false);
        return RGBSpectrum::black;
    }

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const = 0;

    inline static Ref<Material> fallback = nullptr;
};

class Lambertian : public Material
{
public:
    Lambertian(const Spectrum& color);
    Lambertian(const Ref<Texture> albedo);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;
    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const override;

public:
    Ref<Texture> albedo;
};

class Dielectric : public Material
{
public:
    Dielectric(Float index_of_refraction);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;

    Float ior; // Index of Refraction

private:
    static Float Reflectance(Float cosine, Float ref_idx)
    {
        // Schlick's approximation for reflectance.
        Float r0 = Sqr((1 - ref_idx) / (1 + ref_idx));

        return r0 + (1 - r0) * std::pow((1 - cosine), Float(5.0));
    }
};

class Metal : public Material
{
public:
    Metal(const Spectrum& albedo, Float fuzziness);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;

public:
    Spectrum albedo;
    Float fuzziness;
};

// Microfacet material model
// BRDF (Cook-Torrance specular + Lambertian diffuse)
// - GGX normal distribution function
// - Smith-GGX height-correlated visibility function
// - width Schlick Fresnel blend
class Microfacet : public Material
{
public:
    Microfacet(const Ref<Texture> basecolor,
               const Ref<Texture> metallic,
               const Ref<Texture> roughness,
               const Ref<Texture> emissive = ConstantColor::Create(Float(0.0)),
               const Ref<Texture> normalmap = ConstantColor::Create(Float(0.5), Float(0.5), Float(1.0)));

    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;
    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const override;

private:
    Ref<Texture> basecolor, metallic, roughness, emissive, normalmap;
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Spectrum& color, bool two_sided = false);
    DiffuseLight(const Ref<Texture> emission, bool two_sided = false);

    virtual bool IsLightSource() const override;
    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;

    Ref<Texture> emission;
    bool two_sided;
};

} // namespace bulbit