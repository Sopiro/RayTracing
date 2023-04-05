#include "raytracer/pbr_material.h"
#include "raytracer/cosine_pdf.h"
#include "raytracer/ggx_pdf.h"
#include "raytracer/microfacet.h"

namespace spt
{

// Microfacet BRDF (Cook-Torrance specular + Lambertian diffuse)
// GGX normal distribution function
// Smith-GGX height-correlated visibility function
// Schlick Fresnel function
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

    double NoV = Max(Dot(n, v), 0.0);
    double NoL = Max(Dot(n, l), 0.0);
    double NoH = Max(Dot(n, h), 0.0);
    double VoH = Max(Dot(v, h), 0.0);

    Vec3 f0 = F0(basecolor, metallic);
    Vec3 F = F_Schlick(f0, VoH);
    double D = D_GGX(NoH, alpha2);
    double V = V_SmithGGXCorrelated(NoV, NoL, alpha2);

    Vec3 f_s = F * (D * V);
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
    double spec_w = Luma(F);
    double t = Max(spec_w / (diff_w + spec_w), 0.25);
#endif

    // out_srec.pdf = std::make_shared<CosinePDF>(in_rec.normal);
    out_srec.pdf = std::make_shared<GGXPDF>(in_rec.normal, wi, roughness, t);
    out_srec.is_specular = false;
    return true;
}

} // namespace spt
