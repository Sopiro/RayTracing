#pragma once

#include "common.h"
#include "pdf.h"
#include "ray.h"
#include "texture.h"

namespace spt
{

struct HitRecord;

struct ScatterRecord
{
    Ray specular_ray;
    bool is_specular;
    Color attenuation;
    std::shared_ptr<PDF> pdf_ptr;
};

class Material
{
public:
    virtual Color Emitted(const Ray& ray_in, const HitRecord& in_rec, const UV& in_uv, const Vec3& in_p) const
    {
        return Color{ 0.0, 0.0, 0.0 };
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const = 0;

    // BRDF
    virtual double ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
    {
        return 0;
    }
};

class Lambertian : public Material
{
public:
    Lambertian(const Color& _albedo)
        : albedo{ std::make_shared<SolidColor>(_albedo) }
    {
    }
    Lambertian(std::shared_ptr<Texture> _albedo)
        : albedo{ _albedo }
    {
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;
    virtual double ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const override;

public:
    std::shared_ptr<Texture> albedo;
};

class Metal : public Material
{
public:
    Metal(const Color& _albedo, double _fuzziness)
        : albedo{ _albedo }
        , fuzziness{ _fuzziness }
    {
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;

public:
    Color albedo;
    double fuzziness;
};

class Dielectric : public Material
{
public:
    double ir; // Index of Refraction

    Dielectric(double index_of_refraction)
        : ir{ index_of_refraction }
    {
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;

private:
    static double Reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        double r0 = (double(1.0) - ref_idx) / (double(1.0) + ref_idx);
        r0 = r0 * r0;

        return r0 + (double(1.0) - r0) * pow((1 - cosine), double(5.0));
    }
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(std::shared_ptr<Texture> a)
        : emit(a)
    {
    }

    DiffuseLight(Color c)
        : emit(std::make_shared<SolidColor>(c))
    {
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override
    {
        return false;
    }

    virtual Color Emitted(const Ray& ray_in, const HitRecord& in_rec, const UV& in_uv, const Vec3& in_p) const override;

public:
    std::shared_ptr<Texture> emit;
};

class Isotropic : public Material
{
public:
    Isotropic(Color c)
        : albedo(std::make_shared<SolidColor>(c))
    {
    }
    Isotropic(std::shared_ptr<Texture> a)
        : albedo(a)
    {
    }

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;

public:
    std::shared_ptr<Texture> albedo;
};

} // namespace spt