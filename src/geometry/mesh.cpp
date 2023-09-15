#include "spt/mesh.h"
#include "spt/lambertian.h"
#include "spt/triangle.h"

#include "spt/util.h"

namespace spt
{

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<i32> _indices, const Mat4& transform, const Ref<Material> _material)
    : vertices{ std::move(_vertices) }
    , indices{ std::move(_indices) }
    , material{ _material }
{
    // Transform vertices to the world space
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        Vertex& v = vertices[i];

        Vec4 vP = Mul(transform, Vec4(v.position, 1.0));
        Vec4 vN = Mul(transform, Vec4(v.normal, 0.0));
        Vec4 vT = Mul(transform, Vec4(v.tangent, 0.0));
        vN.Normalize();
        vT.Normalize();

        v.position.Set(vP.x, vP.y, vP.z);
        v.normal.Set(vN.x, vN.y, vN.z);
        v.tangent.Set(vT.x, vT.y, vT.z);
    }

    triangle_count = i32(indices.size() / 3);
}

} // namespace spt