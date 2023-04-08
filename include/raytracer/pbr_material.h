#pragma once

#include "material.h"
#include "postprocess.h"

namespace spt
{

class PBRMaterial : public Material
{
public:
    PBRMaterial() = default;

    virtual Color Emit(const Ray& in_ray, const HitRecord& in_rec) const override;
    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;
    virtual Vec3 Evaluate(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const override;

public:
    std::shared_ptr<Texture> basecolor_map;
    std::shared_ptr<Texture> normal_map;
    std::shared_ptr<Texture> metallic_map;
    std::shared_ptr<Texture> roughness_map;
    std::shared_ptr<Texture> ao_map;
    std::shared_ptr<Texture> emissive_map;
};

inline Color PBRMaterial::Emit(const Ray& in_ray, const HitRecord& in_rec) const
{
    return emissive_map->Value(in_rec.uv, in_rec.point);
}

} // namespace spt
