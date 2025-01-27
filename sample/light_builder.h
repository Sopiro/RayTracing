#pragma once

#include "bulbit/lights.h"
#include "bulbit/scene.h"

namespace bulbit
{

PointLight* CreatePointLight(Scene& scene, const Point3& position, const Spectrum& intensity);
PointLight* CreatePointLight(Scene& scene, const Point3& position, Float intensity);

DirectionalLight* CreateDirectionalLight(
    Scene& scene, const Vec3& direction, const Spectrum& intensity, Float visible_radius = 0
);
DirectionalLight* CreateDirectionalLight(Scene& scene, const Vec3& direction, Float intensity, Float visible_radius = 0);

UniformInfiniteLight* CreateUniformInfiniteLight(Scene& scene, const Spectrum& l, Float scale = 1);
UniformInfiniteLight* CreateUniformInfiniteLight(Scene& scene, Float l, Float scale = 1);

ImageInfiniteLight* CreateImageInfiniteLight(
    Scene& scene, const std::string& filename, const Transform& tf = identity, Float scale = 1
);

} // namespace bulbit
