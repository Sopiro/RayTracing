#include "raytracer/pbr_material.h"

namespace spt
{

inline static double Luminance(Vec3 color)
{
    return Dot(color, Vec3(0.299, 0.587, 0.114));
}

constexpr Vec3 default_reflectance{ 0.04 };
constexpr double tolerance = epsilon;

inline static Vec3 F0(Vec3 basecolor, double metallic)
{
    return Lerp(default_reflectance, basecolor, metallic);
}

inline static Vec3 F_Schlick(Vec3 f0, double cosine_theta)
{
    return f0 + (Vec3(1.0) - f0) * pow(1.0 - cosine_theta, 5.0);
}

inline static double D_GGX(double NoH, double alpha2)
{
    double NoH2 = NoH * NoH;
    double b = (NoH2 * (alpha2 - 1.0) + 1.0);
    return alpha2 / (b * b * pi + tolerance);
}

inline static double G_Smith(double NoV, double NoL, double alpha2)
{
    double denomA = NoV * sqrt(alpha2 + (1.0 - alpha2) * NoL * NoL);
    double denomB = NoL * sqrt(alpha2 + (1.0 - alpha2) * NoV * NoV);
    return 2.0 * NoL * NoV / (denomA + denomB);
}

// Microfacet BRDF
Vec3 PBRMaterial::Evaluate(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
{
    Vec3 basecolor = basecolor_map->Value(in_rec.uv, in_rec.point);
    double roughness = roughness_map->Value(in_rec.uv, in_rec.point).x;
    double alpha2 = roughness * roughness;
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

    double VoH = Max(Dot(v, h), 0.0);
    double NoH = Max(Dot(n, h), 0.0);
    double NoV = Max(Dot(n, v), 0.0);
    double NoL = Max(Dot(n, l), 0.0);

    Vec3 f0 = F0(basecolor, metallic);
    Vec3 F = F_Schlick(f0, VoH);
    double D = D_GGX(NoH, alpha2);
    double G = G_Smith(NoV, NoL, alpha2);

    Vec3 f_s = (F * D * G) / (4.0 * NoV * NoL + tolerance);
    Vec3 f_d = (Vec3(1.0) - F) * (1.0 - metallic) * (basecolor / pi);

    return (f_d + f_s) * NoL;
}

bool PBRMaterial::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    Vec3 basecolor = basecolor_map->Value(in_rec.uv, in_rec.point);
    double roughness = roughness_map->Value(in_rec.uv, in_rec.point).x;
    double metallic = metallic_map->Value(in_rec.uv, in_rec.point).x;

    Vec3 wi = in_ray.dir.Normalized();

#if 0
    Vec3 f0 = F0(basecolor, metallic);
    Vec3 diffuse = basecolor * (1.0 - metallic);
    double F0Sum = f0.x + f0.y + f0.z;
    double diffuseSum = diffuse.x + diffuse.y + diffuse.z;
    double t = Max(F0Sum / (diffuseSum + F0Sum), 0.25);
#else
    Vec3 f0 = F0(basecolor, metallic);
    Vec3 F = F_Schlick(f0, Dot(-wi, in_rec.normal));
    double diff_w = (1.0 - metallic);
    double spec_w = Luminance(F);
    double t = Max(spec_w / (diff_w + spec_w), 0.25);
#endif

    // out_srec.pdf = std::make_shared<CosinePDF>(in_rec.normal);
    out_srec.pdf = std::make_shared<GGXPDF>(in_rec.normal, wi, roughness, t);
    out_srec.is_specular = false;
    return true;
}

} // namespace spt
