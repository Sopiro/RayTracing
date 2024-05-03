#include "bulbit/bitmap.h"
#include "bulbit/frame.h"
#include "bulbit/light.h"
#include "bulbit/util.h"

#include <memory>

namespace bulbit
{

ImageInfiniteLight::ImageInfiniteLight(const std::string& env_map, const Transform& tf)
    : ImageInfiniteLight(ColorImageTexture::Create(env_map, false), tf)
{
}

ImageInfiniteLight::ImageInfiniteLight(const ColorImageTexture* l_map, const Transform& tf)
    : Light(Light::Type::infinite_light)
    , l_map{ l_map }
    , transform{ tf }
{
    int32 width = l_map->GetWidth();
    int32 height = l_map->GetHeight();

    std::unique_ptr<Float[]> image(new Float[width * height]);
    for (int32 v = 0; v < height; ++v)
    {
        Float vp = (v + Float(0.5)) / height;
        Float sin_theta = std::sin(pi * vp);

        for (int32 u = 0; u < width; ++u)
        {
            Float up = Float(u) / width;
            image[u + v * width] = (Float)std::fmax(0, sin_theta * l_map->Evaluate(Point2(up, vp)).Luminance());
        }
    }

    distribution.reset(new Distribution2D(image.get(), width, height));
}

Spectrum ImageInfiniteLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const
{
    Float map_pdf;
    Point2 uv = distribution->SampleContinuous(&map_pdf, u);

    if (map_pdf == 0)
    {
        return RGBSpectrum::black;
    }

    Float theta = (1 - uv[1]) * pi;
    Float phi = uv[0] * two_pi;

    Float cos_theta = std::cos(theta), sin_theta = std::sin(theta);
    Float sin_phi = std::sin(phi), cos_phi = std::cos(phi);

    *wi = Mul(transform, Vec3(sin_theta * cos_phi, cos_theta, sin_theta * sin_phi));

    if (sin_theta == 0)
    {
        *pdf = 0;
    }
    else
    {
        *pdf = map_pdf / (2 * pi * pi * sin_theta);
    }

    *visibility = infinity;

    return l_map->Evaluate(uv);
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

Spectrum ImageInfiniteLight::Emit(const Ray& ray) const
{
    Vec3 w = MulT(transform, Normalize(ray.d));
    Float theta = SphericalTheta(w), phi = SphericalPhi(w);
    Point2 uv(phi * inv_two_pi, 1 - theta * inv_pi);

    return l_map->Evaluate(uv);
}

} // namespace bulbit
