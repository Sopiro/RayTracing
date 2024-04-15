#pragma once

#include "image_texture.h"
#include "light.h"
#include "sampling.h"
#include "transform.h"

namespace bulbit
{

class InfiniteAreaLight : public Light
{
public:
    InfiniteAreaLight(const std::string& env_map, const Transform& transform = identity);
    InfiniteAreaLight(const Ref<ImageTexture> l_map, const Transform& transform = identity);

    virtual Light* Clone(Allocator* allocator) const override;

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Spectrum Emit(const Ray& ray) const override;

private:
    Transform transform;

    std::unique_ptr<Distribution2D> distribution;
    Ref<ImageTexture> l_map; // Environment(Radiance) map
};

} // namespace bulbit
