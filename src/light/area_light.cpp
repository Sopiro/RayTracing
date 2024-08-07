#include "bulbit/lights.h"
#include "bulbit/materials.h"

namespace bulbit
{

AreaLight::AreaLight(const Primitive* primitive)
    : Light{ Light::Type::area_light }
    , primitive{ primitive }
{
}

LightSample AreaLight::Sample_Li(const Intersection& ref, const Point2& u) const
{
    ShapeSample shape_sample = primitive->GetShape()->Sample(ref.point, u);
    Vec3 ref2p = shape_sample.point - ref.point;

    LightSample light_sample;
    light_sample.visibility = ref2p.Normalize() - Ray::epsilon;
    light_sample.wi = ref2p;
    light_sample.pdf = shape_sample.pdf;

    Intersection isect;
    isect.point = shape_sample.point;
    isect.front_face = Dot(shape_sample.normal, ref2p) < 0;
    light_sample.Li = primitive->GetMaterial()->Le(isect, ref2p);

    return light_sample;
}

Float AreaLight::EvaluatePDF(const Ray& ray) const
{
    return primitive->GetShape()->EvaluatePDF(ray);
}

Spectrum AreaLight::Le(const Ray& ray) const
{
    assert(false);

    Intersection isect;
    if (!primitive->Intersect(&isect, ray, epsilon, infinity))
    {
        return Spectrum::black;
    }

    return primitive->GetMaterial()->Le(isect, -ray.d);
}

} // namespace bulbit
