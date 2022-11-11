#pragma once

#include "common.h"
#include "ray.h"
#include "texture.h"

struct HitRecord;

class Material
{
public:
    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
    virtual Color Emitted(const UV& uv, const Vec3& p) const
    {
        return Color(0, 0, 0);
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

    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

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

    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

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

    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

private:
    static double Reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
        r0 = r0 * r0;

        return r0 + (1.0 - r0) * pow((1 - cosine), 5.0);
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

    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
    {
        return false;
    }

    virtual Color Emitted(const UV& uv, const Vec3& p) const override
    {
        return emit->Value(uv, p);
    }

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

    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

public:
    std::shared_ptr<Texture> albedo;
};