#include "raytracer/raytracer.h"

namespace spt
{

void NormalMapping(Scene& scene)
{
    Transform transform{ zero_vec3, Quat{ DegToRad(0.0), y_axis }, Vec3{ 0.02 } };
    std::shared_ptr<Model> sponza = std::make_shared<Model>("res/pbr_kabuto_samurai_helmet/scene.gltf", transform);

    scene.Add(sponza);

    scene.SetSkyColor(Color{ 0.7, 0.8, 0.9 });
}

} // namespace spt