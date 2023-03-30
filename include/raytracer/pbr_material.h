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
    virtual double ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const override;
    virtual Vec3 BRDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const override;

public:
    std::shared_ptr<Texture> albedo_map;
    std::shared_ptr<Texture> normal_map;
    std::shared_ptr<Texture> roughness_map;
    std::shared_ptr<Texture> metallic_map;
    std::shared_ptr<Texture> ao_map;
    std::shared_ptr<Texture> emissive_map;
};

inline Color PBRMaterial::Emit(const Ray& in_ray, const HitRecord& in_rec) const
{
    return emissive_map->Value(in_rec.uv, in_rec.point);
}

inline bool PBRMaterial::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    out_srec.is_specular = false;
    out_srec.pdf = std::make_shared<CosinePDF>(in_rec.normal);
    return true;
}

inline double PBRMaterial::ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
{
    // Cosine density
    return Dot(in_rec.normal, in_scattered.dir) / pi;
}

static inline Vec3 F_Schlick(Vec3 f0, double cosine_theta)
{
    return f0 + (Vec3(1.0) - f0) * pow(1.0 - cosine_theta, 5.0);
}

static inline double D_GGX(double NoH, double roughness)
{
    double alpha = roughness * roughness;
    double alpha2 = alpha * alpha;
    double NoH2 = NoH * NoH;

    double b = (NoH2 * (alpha2 - 1.0) + 1.0);

    return alpha2 / (b * b * pi + 0.0001);
}

static inline double G1_GGX_Schlick(double NoV, double roughness)
{
    double alpha = roughness * roughness;
    double k = alpha * 0.5;

    return NoV / (NoV * (1.0 - k) + k + 0.0001);
}

static inline double G_Smith(double NoV, double NoL, double roughness)
{
    return G1_GGX_Schlick(NoV, roughness) * G1_GGX_Schlick(NoL, roughness);
}

inline Vec3 PBRMaterial::BRDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
{
    Vec3 albedo = albedo_map->Value(in_rec.uv, in_rec.point);
    double roughness = roughness_map->Value(in_rec.uv, in_rec.point).x;
    double metallic = metallic_map->Value(in_rec.uv, in_rec.point).x;
    Vec3 ao = ao_map->Value(in_rec.uv, in_rec.point);
    Vec3 emissive = emissive_map->Value(in_rec.uv, in_rec.point);
    Vec3 normal = normal_map->Value(in_rec.uv, in_rec.point) * 2.0 - Vec3(1.0);
    normal.Normalize();

    ONB tbn;
    tbn.u = in_rec.tangent;
    tbn.w = in_rec.normal;
    tbn.v = Cross(tbn.w, tbn.u);

    Vec3 n = tbn.GetLocal(normal);
    Vec3 v = -in_ray.dir.Normalized();
    Vec3 l = in_scattered.dir.Normalized();
    Vec3 h = (v + l).Normalized();

    double VoH = Clamp(Dot(v, h), 0.0, 1.0);
    double NoH = Clamp(Dot(n, h), 0.0, 1.0);
    double NoV = Clamp(Dot(n, v), 0.0, 1.0);
    double NoL = Clamp(Dot(n, l), 0.0, 1.0);

    Vec3 f0 = Lerp(Vec3(0.04), albedo, metallic);
    Vec3 F = F_Schlick(f0, VoH);
    double D = D_GGX(NoH, roughness);
    double G = G_Smith(NoV, NoL, roughness);

    Vec3 f_s = (F * D * G) / (4.0 * NoV * NoL + 0.0001);
    Vec3 f_d = (Vec3(1.0) - F) * (1.0 - metallic) * (albedo / pi);

    return (f_d + f_s) * NoL;
}

} // namespace spt
