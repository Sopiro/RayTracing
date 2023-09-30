#pragma once

#include "light.h"
#include "sampling.h"
#include "texture.h"

namespace spt
{

class InfiniteAreaLight : public Light
{
public:
    InfiniteAreaLight(const Ref<Texture> tex_map);

    virtual Color Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Color Emit(const Ray& ray) const override;

private:
    Ref<Texture> l_map; // Environment(Radiance) map
};

} // namespace spt
