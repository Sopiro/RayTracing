#include "spt/infinite_area_light.h"
#include "spt/bitmap.h"
#include "spt/image_texture.h"
#include "spt/util.h"

#include <memory>

namespace spt
{

InfiniteAreaLight::InfiniteAreaLight(const std::string& env_map, bool srgb)
    : Light(Light::Type::infinite_area_light)
    , l_map{ ImageTexture::Create(env_map, srgb) }
    , transform{ Quat(-pi / Float(2.0), x_axis) }
{
    int32 width = l_map->GetWidth();
    int32 height = l_map->GetHeight();

    std::unique_ptr<Float[]> image(new Float[width * height]);
    for (int32 v = 0; v < height; ++v)
    {
        Float vp = (Float)v / (Float)height;
        Float sin_theta = std::sin(pi * (v + Float(0.5)) / Float(height));

        for (int32 u = 0; u < width; ++u)
        {
            Float up = (Float)u / (Float)width;
            image[u + v * width] = sin_theta * Luma(l_map->Value(Point2(up, vp)));
        }
    }

    distribution.reset(new Distribution2D(image.get(), width, height));
}

Color InfiniteAreaLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const
{
    Float map_pdf;
    Point2 uv = distribution->SampleContinuous(&map_pdf, RandVec2());

    if (map_pdf == 0)
    {
        return Color(0, 0, 0);
    }

    Float theta = (1 - uv[1]) * pi;
    Float phi = uv[0] * two_pi;

    Float cos_theta = std::cos(theta), sin_theta = std::sin(theta);
    Float sin_phi = std::sin(phi), cos_phi = std::cos(phi);

    *wi = Mul(transform, Vec3(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta));

    if (sin_theta == 0)
    {
        *pdf = 0;
    }
    else
    {
        *pdf = map_pdf / (2 * pi * pi * sin_theta);
    }

    *visibility = infinity;

    return l_map->Value(uv);
}

Float InfiniteAreaLight::EvaluatePDF(const Ray& ray) const
{
    Vec3 w = MulT(transform, ray.d);
    Float theta = SphericalTheta(w), phi = SphericalPhi(w);
    Float sin_theta = std::sin(theta);
    if (sin_theta == 0)
    {
        return 0;
    }

    Point2 uv(phi * inv_two_pi, 1 - theta * inv_pi);
    return distribution->Pdf(uv) / (2 * pi * pi * sin_theta);
}

Color InfiniteAreaLight::Emit(const Ray& ray) const
{
    Vec3 w = MulT(transform, ray.d);
    Float theta = SphericalTheta(w), phi = SphericalPhi(w);
    Point2 uv(phi * inv_two_pi, 1 - theta * inv_pi);

    return l_map->Value(uv);
}

} // namespace spt
