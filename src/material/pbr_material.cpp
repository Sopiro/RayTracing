#include "spt/pbr_material.h"
#include "spt/cosine_pdf.h"
#include "spt/ggx_pdf.h"
#include "spt/ggxvndf_pdf.h"
#include "spt/microfacet.h"

namespace spt
{

// Microfacet BRDF (Cook-Torrance specular + Lambertian diffuse)
// GGX normal distribution function
// Smith-GGX height-correlated visibility function
// Schlick Fresnel function
Vec3 PBRMaterial::Evaluate(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
{
    Vec3 normal = normal_map->Value(in_rec.uv, in_rec.point) * 2.0 - Vec3{ 1.0 };
    normal.Normalize();

    ONB tbn;
    tbn.u = in_rec.tangent;
    tbn.w = in_rec.normal;
    tbn.v = Cross(tbn.w, tbn.u).Normalized();

    Vec3 n = tbn.GetLocal(normal).Normalized(); // normal
    Vec3 v = -in_ray.dir.Normalized();          // incident
    Vec3 l = in_scattered.dir.Normalized();     // outgoing
    Vec3 h = v + l;                             // half

    // Resolve back facing shading normal by flipping method
    if (Dot(n, v) < 0.0)
    {
        n = Reflect(-n, in_rec.normal);
    }

    double NoV = Dot(n, v);
    double NoL = Dot(n, l);

    if (NoV <= 0.0 || NoL <= 0.0 || h == zero_vec3)
    {
        return zero_vec3;
    }

    h.Normalize();

    double NoH = Dot(n, h);
    double VoH = Dot(v, h);

    Vec3 basecolor = basecolor_map->Value(in_rec.uv, in_rec.point);
    double metallic = metallic_map->Value(in_rec.uv, in_rec.point).z;
    double roughness = roughness_map->Value(in_rec.uv, in_rec.point).y;
    double ao = ao_map->Value(in_rec.uv, in_rec.point).x;
    Vec3 emissive = emissive_map->Value(in_rec.uv, in_rec.point);

    double alpha = fmax(roughness, min_roughness);
    double alpha2 = alpha * alpha;

    Vec3 f0 = F0(basecolor, metallic);
    Vec3 F = F_Schlick(f0, VoH);
    double D = D_GGX(NoH, alpha2);
    double V = V_SmithGGXCorrelated(NoV, NoL, alpha2);
    // double G = G2_Smith(NoV, NoL, alpha2);

    Vec3 f_s = F * (D * V);
    // Vec3 f_s = F * (D * G) / (4.0 * NoV * NoL);
    Vec3 f_d = (Vec3(1.0) - F) * (1.0 - metallic) * (basecolor * inv_pi);

    return (f_d + f_s) * NoL;
}

bool PBRMaterial::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    Vec3 basecolor = basecolor_map->Value(in_rec.uv, in_rec.point);
    double metallic = metallic_map->Value(in_rec.uv, in_rec.point).z;
    double roughness = roughness_map->Value(in_rec.uv, in_rec.point).y;

    double alpha = fmax(roughness, min_roughness);
    Vec3 wo = -in_ray.dir.Normalized();

#if 0
    Vec3 f0 = F0(basecolor, metallic);
    Vec3 diffuse = basecolor * (1.0 - metallic);
    double F0Sum = f0.x + f0.y + f0.z;
    double diffuseSum = diffuse.x + diffuse.y + diffuse.z;
    double t = fmax(F0Sum / (diffuseSum + F0Sum), 0.25);
#else
    Vec3 f0 = F0(basecolor, metallic);
    Vec3 F = F_Schlick(f0, Dot(wo, in_rec.normal));
    double diff_w = (1.0 - metallic);
    double spec_w = Luma(F);
    // double spec_w = fmax(F.x, fmax(F.y, F.z));
    double t = fmax(spec_w / (diff_w + spec_w), 0.25);
#endif

    // out_srec.pdf = SharedRef<CosinePDF>(in_rec.normal);
    // out_srec.pdf = SharedRef<GGXPDF>(in_rec.normal, wo, alpha, t);
    out_srec.pdf = CreateSharedRef<GGXVNDFPDF>(in_rec.normal, wo, alpha, t);
    out_srec.is_specular = false;
    return true;
}

} // namespace spt
