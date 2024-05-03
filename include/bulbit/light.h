#pragma once

#include "intersectable.h"
#include "primitive.h"
#include "sampling.h"
#include "spectrum.h"
#include "texture.h"
#include "transform.h"

namespace bulbit
{

class Light
{
public:
    enum class Type
    {
        point_light = 0,
        directional_light,
        area_light,
        infinite_light,
    };

    Light(Type type)
        : type{ type }
    {
    }

    virtual ~Light() = default;

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const = 0;
    virtual Float EvaluatePDF(const Ray& ray) const = 0;

    virtual Spectrum Emit(const Ray& ray) const
    {
        return RGBSpectrum::black;
    }

    bool IsDeltaLight() const
    {
        return type == Type::point_light || type == Type::directional_light;
    }

    const Type type;
};

class PointLight : public Light
{
public:
    PointLight(const Point3& position, const Spectrum& intensity);

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const override;

    virtual Float EvaluatePDF(const Ray& ray) const override
    {
        assert(false);
        return Float(0.0);
    }

    Point3 position;
    Spectrum intensity; // radiance
};

struct DirectionalLight : public Light
{
public:
    DirectionalLight(const Vec3& dir, const Spectrum& intensity, Float radius);

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const override;

    virtual Float EvaluatePDF(const Ray& ray) const override
    {
        assert(false);
        return Float(0.0);
    }

    Vec3 dir;
    Spectrum intensity; // radiance
    Float radius;       // visible radius
};

class AreaLight : public Light
{
public:
    AreaLight(const Primitive* primitive);

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const override;

    virtual Float EvaluatePDF(const Ray& ray) const override
    {
        return primitive->EvaluatePDF(ray);
    }

    const Primitive* GetPrimitive() const
    {
        return primitive;
    }

private:
    friend class Scene;

    const Primitive* primitive;
    const Material* material;
};

class ImageInfiniteLight : public Light
{
public:
    ImageInfiniteLight(const std::string& env_map, const Transform& transform = identity);
    ImageInfiniteLight(const ColorImageTexture* l_map, const Transform& transform = identity);

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Spectrum Emit(const Ray& ray) const override;

private:
    Transform transform;

    std::unique_ptr<Distribution2D> distribution;
    const ColorImageTexture* l_map; // Environment(Radiance) map
};

class UniformInfiniteLight : public Light
{
public:
    UniformInfiniteLight(const Spectrum& l, Float scale = 1);

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Spectrum Emit(const Ray& ray) const override;

private:
    Spectrum l;
    Float scale;
};

} // namespace bulbit
