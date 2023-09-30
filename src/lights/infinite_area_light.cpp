#include "spt/infinite_area_light.h"
#include "spt/image_texture.h"

namespace spt
{

InfiniteAreaLight::InfiniteAreaLight(const Ref<Texture> tex_map)
    : Light(Light::Type::infinite_area_light)
    , l_map{ tex_map }
{
}

Color InfiniteAreaLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const
{
    *wi = UniformSampleSphere();
    *pdf = UniformSampleSpherePDF();
    *visibility = infinity;

    UV uv = ComputeSphereTexCoord(*wi);

    return l_map->Value(uv);
}

Float InfiniteAreaLight::EvaluatePDF(const Ray& ray) const
{
    return UniformSampleSpherePDF();
}

Color InfiniteAreaLight::Emit(const Ray& ray) const
{
    UV uv = ComputeSphereTexCoord(Normalize(ray.d));
    return l_map->Value(uv);
}

} // namespace spt
