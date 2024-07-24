#pragma once

#include "light.h"

namespace bulbit
{

class PointLight : public Light
{
public:
    PointLight(const Point3& position, const Spectrum& intensity);

    virtual LightSample Sample_Li(const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

private:
    Point3 position;
    Spectrum intensity; // radiance
};

class DirectionalLight : public Light
{
public:
    DirectionalLight(const Vec3& dir, const Spectrum& intensity, Float radius);

    virtual LightSample Sample_Li(const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

private:
    Vec3 dir;
    Spectrum intensity; // radiance
    Float radius;       // visible radius
};

class AreaLight : public Light
{
public:
    AreaLight(const Primitive* primitive);

    virtual LightSample Sample_Li(const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

    virtual Spectrum Le(const Ray& ray) const override;

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
    ImageInfiniteLight(const std::string& env_map, const Transform& transform = identity);
    ImageInfiniteLight(const ColorImageTexture* l_map, const Transform& transform = identity);

    virtual LightSample Sample_Li(const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

    virtual Spectrum Le(const Ray& ray) const override;

private:
    Transform transform;

    std::unique_ptr<Distribution2D> distribution;
    const ColorImageTexture* l_map; // Environment(Radiance) map
};

class UniformInfiniteLight : public Light
{
public:
    UniformInfiniteLight(const Spectrum& l, Float scale = 1);

    virtual LightSample Sample_Li(const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

    virtual Spectrum Le(const Ray& ray) const override;

private:
    Spectrum l;
    Float scale;
};

} // namespace bulbit
