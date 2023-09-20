#pragma once

#include "common.h"
#include "intersectable.h"

namespace spt
{

class Light
{
public:
    enum class Type
    {
        point_light = 0,
        directional_light,
        area_light,
    };

    Light(Type type);

    virtual ~Light() = default;

    virtual Color Sample(Vec3* wi, f64* pdf, f64* visibility, const Intersection& ref) const = 0;
    virtual f64 EvaluatePDF(const Ray& ray) const = 0;

    bool IsDeltaLight() const;

    const Type type;
};

inline Light::Light(Type _type)
    : type{ _type }
{
}

inline bool Light::IsDeltaLight() const
{
    return type == Type::point_light || type == Type::directional_light;
}

} // namespace spt
