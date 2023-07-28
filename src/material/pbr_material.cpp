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
    Vec3 normal = normal_map->Value(in_rec.uv, in_rec.point) * 2.0 - Vec3(1.0);
    normal.Normalize();

    ONB tbn;
    tbn.u = in_rec.tangent;
    tbn.w = in_rec.normal;
    tbn.v = Cross(tbn.w, tbn.u);

    Vec3 n = tbn.GetLocal(normal).Normalized(); // normal
    Vec3 v = -in_ray.dir.Normalized();          // incident
    Vec3 l = in_scattered.dir.Normalized();     // outgoing
    Vec3 h = v + l;                             // half

    // Resolve back facing shading normal by flipping method
    if (Dot(n, v) < 0.0)
    {
        n = Reflect(-n, in_rec.normal);
    }

    f64 NoV = Dot(n, v);
    f64 NoL = Dot(n, l);

    if (NoV <= 0.0 || NoL <= 0.0 || h == zero_vec3)
    {
        return zero_vec3;
    }

    h.Normalize();

    f64 NoH = Dot(n, h);
    f64 VoH = Dot(v, h);

    Vec3 basecolor = basecolor_map->Value(in_rec.uv, in_rec.point);
    f64 metallic = metallic_map->Value(in_rec.uv, in_rec.point).z;
    f64 roughness = roughness_map->Value(in_rec.uv, in_rec.point).y;
    f64 ao = ao_map->Value(in_rec.uv, in_rec.point).x;

    f64 alpha = fmax(roughness, min_roughness);
    f64 alpha2 = alpha * alpha;

    Vec3 f0 = F0(basecolor, metallic);
    Vec3 F = F_Schlick(f0, VoH);
    f64 D = D_GGX(NoH, alpha2);
    f64 V = V_SmithGGXCorrelated(NoV, NoL, alpha2);
    // float64 G = G2_Smith(NoV, NoL, alpha2);

    Vec3 f_s = F * (D * V);
    // Vec3 f_s = F * (D * G) / (4.0 * NoV * NoL);
    Vec3 f_d = (Vec3(1.0) - F) * (1.0 - metallic) * (basecolor * inv_pi);

    return (f_d + f_s) * NoL;
}

bool PBRMaterial::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    Vec3 basecolor = basecolor_map->Value(in_rec.uv, in_rec.point);
    f64 metallic = metallic_map->Value(in_rec.uv, in_rec.point).z;
    f64 roughness = roughness_map->Value(in_rec.uv, in_rec.point).y;

    f64 alpha = fmax(roughness, min_roughness);
    Vec3 wo = -in_ray.dir.Normalized();

    Vec3 f0 = F0(basecolor, metallic);
    Vec3 F = F_Schlick(f0, Dot(wo, in_rec.normal));
    f64 diff_w = (1.0 - metallic);
    f64 spec_w = Luma(F);
    // float64 spec_w = fmax(F.x, fmax(F.y, F.z));
    f64 t = Clamp(spec_w / (diff_w + spec_w), 0.25, 0.9);

    // out_srec.pdf = CreateSharedRef<CosinePDF>(in_rec.normal);
    // out_srec.pdf = CreateSharedRef<GGXPDF>(in_rec.normal, wo, alpha, t);
    out_srec.pdf = CreateSharedRef<GGXVNDFPDF>(in_rec.normal, wo, alpha, t);
    out_srec.is_specular = false;

    return true;
}

} // namespace spt
