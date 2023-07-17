#pragma once

#include "material.h"
#include "postprocess.h"

namespace spt
{

// Microfacet material
class PBRMaterial : public Material
{
public:
    PBRMaterial() = default;

    virtual Color Emit(const Ray& in_ray, const HitRecord& in_rec) const override;
    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;
    virtual Vec3 Evaluate(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const override;

public:
    Ref<Texture> basecolor_map;
    Ref<Texture> normal_map;
    Ref<Texture> metallic_map;
    Ref<Texture> roughness_map;
    Ref<Texture> ao_map;
    Ref<Texture> emissive_map;
};

inline Color PBRMaterial::Emit(const Ray& in_ray, const HitRecord& in_rec) const
{
    return emissive_map->Value(in_rec.uv, in_rec.point);
}

} // namespace spt
