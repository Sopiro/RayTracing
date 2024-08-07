#pragma once

#include "dynamic_dispatcher.h"
#include "ray.h"
#include "spectrum.h"

namespace bulbit
{

struct Intersection;

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

using Lights =
    TypePack<class PointLight, class DirectionalLight, class AreaLight, class UniformInfiniteLight, class ImageInfiniteLight>;

class Light : public DynamicDispatcher<Lights>
{
protected:
    Light(int8 type_index)
        : DynamicDispatcher(type_index)
    {
    }

public:
    LightSample Sample_Li(const Intersection& ref, const Point2& u) const;
    Float EvaluatePDF(const Ray& ray) const;
    Spectrum Le(const Ray& ray) const;

    bool IsDeltaLight() const;
};

} // namespace bulbit
