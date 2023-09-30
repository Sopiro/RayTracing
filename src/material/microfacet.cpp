#include "spt/microfacet.h"

#include "spt/cosine_pdf.h"
#include "spt/ggx_pdf.h"
#include "spt/ggxvndf_pdf.h"

#include "spt/util.h"

namespace spt
{

bool Microfacet::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi) const
{
    Vec3 c = basecolor->Value(is.uv);
    Float m = metallic->Value(is.uv).z;
    Float r = roughness->Value(is.uv).y;

    Float alpha = RoughnessToAlpha(r);
    Vec3 wo = -wi;

    Vec3 f0 = F0(c, m);
    Vec3 F = F_Schlick(f0, Dot(wo, is.shading.normal));
    Float diff_w = (1 - m);
    Float spec_w = Luma(F);
    // Float spec_w = std::fmax(F.x, std::fmax(F.y, F.z));
    Float t = Clamp(spec_w / (diff_w + spec_w), Float(0.15), Float(0.9));

    // new (ir->mem) CosinePDF(is.shading.normal);
    // new (ir->mem) GGXPDF(is.shading.normal, wo, alpha, t);
    new (ir->mem) GGXVNDFPDF(is.shading.normal, wo, alpha, t);
    ir->is_specular = false;

    return true;
}

Vec3 Microfacet::Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const
{
    Vec3 normal = normal_map->Value(is.uv) * 2.0 - Vec3(1.0);
    normal.Normalize();

    ONB tbn;
    tbn.u = is.shading.tangent;
    tbn.w = is.shading.normal;
    tbn.v = Cross(tbn.w, tbn.u);
    tbn.v.Normalize();

    Vec3 n = Normalize(tbn.GetLocal(normal)); // normal
    Vec3 v = -wi;                             // incident
    Vec3 l = wo;                              // outgoing
    Vec3 h = v + l;                           // half

    // Resolve back facing shading normal by flipping method
    if (Dot(n, v) < Float(0.0))
    {
        n = Reflect(n, is.shading.normal);
    }

    Float NoV = Dot(n, v);
    Float NoL = Dot(n, l);

    if (NoV <= Float(0.0) || NoL <= Float(0.0) || h == zero_vec3)
    {
        return zero_vec3;
    }

    h.Normalize();

    Float NoH = Dot(n, h);
    Float VoH = Dot(v, h);

    Vec3 c = basecolor->Value(is.uv);
    Float m = metallic->Value(is.uv).z;
    Float r = roughness->Value(is.uv).y;

    Float alpha = RoughnessToAlpha(r);
    Float alpha2 = alpha * alpha;

    Vec3 f0 = F0(c, m);
    Vec3 F = F_Schlick(f0, VoH);
    Float D = D_GGX(NoH, alpha2);
    // Float G = G2_Smith_Correlated(NoV, NoL, alpha2);
    Float V = V_Smith_Correlated(NoV, NoL, alpha2);

    Vec3 f_s = F * (D * V);
    // Vec3 f_s = F * (D * G) / (Float(4.0) * NoV * NoL);
    Vec3 f_d = (Vec3(1) - F) * (1 - m) * (c * inv_pi);

    return (f_d + f_s) * NoL;
}

} // namespace spt
