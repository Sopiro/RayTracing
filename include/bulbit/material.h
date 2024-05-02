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
    virtual ~Material() = default;

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

    virtual bool TestAlpha(const Point2& uv) const
    {
        return true;
    }
};

class Lambertian : public Material
{
public:
    Lambertian(const Spectrum& color);
    Lambertian(const Texture* albedo);

    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;
    virtual bool TestAlpha(const Point2& uv) const override;

public:
    const Texture* albedo;
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

    static Vec3 Refract(const Vec3& uv, const Vec3& n, Float eta_p /*== eta_i / eta_t*/)
    {
        Float cos_theta = std::fmin(Dot(-uv, n), 1);

        Vec3 r_out_perp = eta_p * (uv + cos_theta * n);
        Vec3 r_out_parl = -std::sqrt(std::fabs(1 - r_out_perp.Length2())) * n;

        return r_out_perp + r_out_parl;
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
    Microfacet(const Texture* basecolor,
               const Texture* metallic,
               const Texture* roughness,
               const Texture* emissive = ConstantColor::Create(Float(0.0)),
               const Texture* normalmap = ConstantColor::Create(Float(0.5), Float(0.5), Float(1.0)));

    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const override;
    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;
    virtual bool TestAlpha(const Point2& uv) const override;

private:
    const Texture* basecolor;
    const Texture* metallic;
    const Texture* roughness;
    const Texture* emissive;
    const Texture* normalmap;
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Spectrum& color, bool two_sided = false);
    DiffuseLight(const Texture* emission, bool two_sided = false);

    virtual bool IsLightSource() const override;

    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;

    const Texture* emission;
    bool two_sided;
};

} // namespace bulbit