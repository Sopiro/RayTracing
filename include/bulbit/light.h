#pragma once

#include "intersectable.h"
#include "spectrum.h"

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
        infinite_area_light,
    };

    Light(Type type);
    virtual ~Light() = default;

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const = 0;
    virtual Float EvaluatePDF(const Ray& ray) const = 0;
    virtual Spectrum Emit(const Ray& ray) const;

    bool IsDeltaLight() const;

    const Type type;
};

inline Light::Light(Type _type)
    : type{ _type }
{
}

inline Spectrum Light::Emit(const Ray& ray) const
{
    return RGBSpectrum::black;
}

inline bool Light::IsDeltaLight() const
{
    return type == Type::point_light || type == Type::directional_light;
}

} // namespace bulbit
