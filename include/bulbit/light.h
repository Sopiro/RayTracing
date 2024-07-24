#pragma once

#include "intersectable.h"
#include "primitive.h"
#include "sampling.h"
#include "spectrum.h"
#include "texture.h"
#include "transform.h"

namespace bulbit
{

struct LightSample
{
    LightSample() = default;

    LightSample(Vec3 wi, Float pdf, Float visibility, Spectrum li)
        : wi{ wi }
        , pdf{ pdf }
        , visibility{ visibility }
    {
    }

    Vec3 wi;
    Float pdf;
    Float visibility;
    Spectrum Li;
};

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

    virtual LightSample Sample_Li(const Intersection& ref, const Point2& u) const = 0;
    virtual Float EvaluatePDF(const Ray& ray) const = 0;

    virtual Spectrum Le(const Ray& ray) const
    {
        return Spectrum::black;
    }

    bool IsDeltaLight() const
    {
        return type == Type::point_light || type == Type::directional_light;
    }

    const Type type;
};

} // namespace bulbit
