#pragma once

#include "distributions.h"
#include "image_texture.h"
#include "light.h"
#include "sampling.h"
#include "transform.h"

namespace spt
{

class InfiniteAreaLight : public Light
{
public:
    InfiniteAreaLight(const std::string& env_map, bool srgb = false);

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Spectrum Emit(const Ray& ray) const override;

private:
    Transform transform;
    std::unique_ptr<Distribution2D> distribution;
    Ref<ImageTexture> l_map; // Environment(Radiance) map
};

} // namespace spt
