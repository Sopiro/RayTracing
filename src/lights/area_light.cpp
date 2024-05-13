#include "bulbit/light.h"
#include "bulbit/material.h"

namespace bulbit
{

AreaLight::AreaLight(const Primitive* primitive)
    : Light{ Light::Type::area_light }
    , primitive{ primitive }
{
}

LightSample AreaLight::Sample(const Intersection& ref, const Point2& u) const
{
    PrimitiveSample ps = primitive->Sample(ref.point, u);
    Vec3 ref2p = ps.point - ref.point;

    LightSample ls;
    ls.visibility = ref2p.Normalize() - Ray::epsilon;
    ls.wi = ref2p;
    ls.pdf = ps.pdf;

    Intersection is;
    is.point = ps.point;
    is.front_face = Dot(ps.normal, ref2p) < 0;
    ls.li = material->Le(is, ref2p);

    return ls;
}

Float AreaLight::EvaluatePDF(const Ray& ray) const
{
    return primitive->EvaluatePDF(ray);
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
