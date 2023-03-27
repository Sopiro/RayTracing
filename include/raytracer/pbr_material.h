#pragma once

#include "material.h"
#include "postprocess.h"

namespace spt
{

class PBRMaterial : public Material
{
public:
    PBRMaterial() = default;

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;
    virtual double ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const override;

public:
    std::shared_ptr<Texture> albedo;
    std::shared_ptr<Texture> normal;
    std::shared_ptr<Texture> roughness;
    std::shared_ptr<Texture> metallic;
    std::shared_ptr<Texture> ao;
    std::shared_ptr<Texture> emissive;
};

inline bool PBRMaterial::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    Vec3 sampled_albedo = albedo->Value(in_rec.uv, in_rec.point);
    Vec3 sampled_normal = normal->Value(in_rec.uv, in_rec.point) * 2.0 - Vec3{ 1.0 };
    sampled_normal.Normalize();

    ONB tbn;
    tbn.u = in_rec.tangent;
    tbn.w = in_rec.normal;
    tbn.v = Cross(tbn.w, tbn.u);

    Vec3 normal = tbn.GetLocal(sampled_normal);

    out_srec.is_specular = false;
    out_srec.attenuation = sampled_albedo;
    out_srec.pdf = std::make_shared<CosinePDF>(normal);

    return true;
}

inline double PBRMaterial::ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
{
    // Cosine density
    return Dot(in_rec.normal, in_scattered.dir) / pi;
}

} // namespace spt
