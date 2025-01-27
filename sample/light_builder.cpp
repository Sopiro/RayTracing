#include "light_builder.h"
#include "texture_builder.h"

namespace bulbit
{

PointLight* CreatePointLight(Scene& scene, const Point3& position, const Spectrum& intensity)
{
    return scene.CreateLight<PointLight>(position, intensity);
}

PointLight* CreatePointLight(Scene& scene, const Point3& position, Float intensity)
{
    return scene.CreateLight<PointLight>(position, Spectrum{ intensity });
}

DirectionalLight* CreateDirectionalLight(Scene& scene, const Vec3& direction, const Spectrum& intensity, Float visible_radius)
{
    return scene.CreateLight<DirectionalLight>(direction, intensity, visible_radius);
}

DirectionalLight* CreateDirectionalLight(Scene& scene, const Vec3& direction, Float intensity, Float visible_radius)
{
    return scene.CreateLight<DirectionalLight>(direction, Spectrum{ intensity }, visible_radius);
}

UniformInfiniteLight* CreateUniformInfiniteLight(Scene& scene, const Spectrum& l, Float scale)
{
    return scene.CreateLight<UniformInfiniteLight>(l, scale);
}

UniformInfiniteLight* CreateUniformInfiniteLight(Scene& scene, Float l, Float scale)
{
    return scene.CreateLight<UniformInfiniteLight>(Spectrum{ l }, scale);
}

ImageInfiniteLight* CreateImageInfiniteLight(Scene& scene, const std::string& filename, const Transform& tf, Float scale)
{
    return scene.CreateLight<ImageInfiniteLight>(CreateSpectrumImageTexture(scene, filename, false), tf, scale);
}

} // namespace bulbit
