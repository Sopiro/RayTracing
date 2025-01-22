#pragma once

#include "intersectable.h"
#include "light.h"
#include "primitive.h"
#include "spectrum.h"
#include "textures.h"
#include "transform.h"

namespace bulbit
{

class PointLight : public Light
{
public:
    PointLight(const Point3& position, const Spectrum& intensity);

    LightSample Sample_Li(const Intersection& ref, const Point2& u) const;
    Float EvaluatePDF(const Ray& ray) const;

private:
    Point3 position;
    Spectrum intensity; // radiance
};

class DirectionalLight : public Light
{
public:
    DirectionalLight(const Vec3& dir, const Spectrum& intensity, Float radius);

    LightSample Sample_Li(const Intersection& ref, const Point2& u) const;
    Float EvaluatePDF(const Ray& ray) const;

private:
    Vec3 dir;
    Spectrum intensity; // radiance
    Float radius;       // visible radius
};

class AreaLight : public Light
{
public:
    AreaLight(const Primitive* primitive);

    LightSample Sample_Li(const Intersection& ref, const Point2& u) const;
    Float EvaluatePDF(const Ray& ray) const;

    Spectrum Le(const Ray& ray) const;

    const Primitive* GetPrimitive() const
    {
        return primitive;
    }

private:
    const Primitive* primitive;
};

class ImageInfiniteLight : public Light
{
public:
    ImageInfiniteLight(const SpectrumImageTexture* l_map, const Transform& transform = identity);

    LightSample Sample_Li(const Intersection& ref, const Point2& u) const;
    Float EvaluatePDF(const Ray& ray) const;

    Spectrum Le(const Ray& ray) const;

private:
    Transform transform;

    std::unique_ptr<Distribution2D> distribution;
    const SpectrumImageTexture* l_map; // Environment(Radiance) map
};

class UniformInfiniteLight : public Light
{
public:
    UniformInfiniteLight(const Spectrum& l, Float scale = 1);

    LightSample Sample_Li(const Intersection& ref, const Point2& u) const;
    Float EvaluatePDF(const Ray& ray) const;

    Spectrum Le(const Ray& ray) const;

private:
    Spectrum l;
    Float scale;
};

inline LightSample Light::Sample_Li(const Intersection& ref, const Point2& u) const
{
    return Dispatch([&](auto light) { return light->Sample_Li(ref, u); });
}

inline Float Light::EvaluatePDF(const Ray& ray) const
{
    return Dispatch([&](auto light) { return light->EvaluatePDF(ray); });
}

inline Spectrum Light::Le(const Ray& ray) const
{
    return Dispatch([&](auto light) { return light->Le(ray); });
}

inline bool Light::IsDeltaLight() const
{
    return Is<PointLight>() || Is<DirectionalLight>();
}

} // namespace bulbit
