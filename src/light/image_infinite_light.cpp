#include "bulbit/frame.h"
#include "bulbit/lights.h"

#include <memory>

namespace bulbit
{

ImageInfiniteLight::ImageInfiniteLight(const SpectrumImageTexture* l_map, const Transform& tf, Float l_scale)
    : Light(TypeIndexOf<ImageInfiniteLight>())
    , l_map{ l_map }
    , l_scale{ l_scale }
    , transform{ tf }
{
    int32 width = l_map->GetWidth();
    int32 height = l_map->GetHeight();

    std::unique_ptr<Float[]> image(new Float[width * height]);
    for (int32 v = 0; v < height; ++v)
    {
        Float vp = (v + 0.5f) / height;
        Float sin_theta = std::sin(pi * vp);

        for (int32 u = 0; u < width; ++u)
        {
            Float up = Float(u) / width;
            image[u + v * width] = (Float)std::fmax(0, sin_theta * l_map->Evaluate(Point2(up, vp)).Luminance());
        }
    }

    distribution = std::make_unique<Distribution2D>(image.get(), width, height);
}

void ImageInfiniteLight::Destroy()
{
    distribution.reset();
}

LightSample ImageInfiniteLight::Sample_Li(const Intersection& ref, const Point2& u) const
{
    BulbitNotUsed(ref);

    Float map_pdf;
    Point2 uv = distribution->SampleContinuous(&map_pdf, u);

    if (map_pdf == 0)
    {
        return LightSample{ Vec3::zero, 0, 0, Spectrum::black };
    }

    Float theta = (1 - uv[1]) * pi;
    Float phi = uv[0] * two_pi;

    Float cos_theta = std::cos(theta), sin_theta = std::sin(theta);
    Float sin_phi = std::sin(phi), cos_phi = std::cos(phi);

    LightSample light_sample;
    light_sample.wi = Mul(transform, SphericalDirection(sin_theta, cos_theta, sin_phi, cos_phi));

    if (sin_theta == 0)
    {
        light_sample.pdf = 0;
    }
    else
    {
        light_sample.pdf = map_pdf / (2 * pi * pi * sin_theta);
    }

    light_sample.visibility = infinity;
    light_sample.Li = l_scale * l_map->Evaluate(uv);

    return light_sample;
}

Float ImageInfiniteLight::EvaluatePDF(const Ray& ray) const
{
    Vec3 w = MulT(transform, Normalize(ray.d));
    Float theta = SphericalTheta(w), phi = SphericalPhi(w);
    Float sin_theta = std::sin(theta);
    if (sin_theta == 0)
    {
        return 0;
    }

    Point2 uv(phi * inv_two_pi, 1 - theta * inv_pi);
    return distribution->Pdf(uv) / (2 * pi * pi * sin_theta);
}

Spectrum ImageInfiniteLight::Le(const Ray& ray) const
{
    Vec3 w = MulT(transform, Normalize(ray.d));
    Float theta = SphericalTheta(w), phi = SphericalPhi(w);
    Point2 uv(phi * inv_two_pi, 1 - theta * inv_pi);

    return l_scale * l_map->Evaluate(uv);
}

} // namespace bulbit
