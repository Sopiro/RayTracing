#pragma once

#include "common.h"
#include "mesh.h"
#include "transform.h"

namespace spt
{

class Model : public Intersectable
{
public:
    Model(const std::string& path, const Transform& transform);

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool GetAABB(AABB* out_aabb) const override;
    virtual i32 GetSize() const override;

    size_t GetMeshCount() const;
    const std::vector<Ref<Mesh>>& GetMeshes();

private:
    std::vector<Ref<Texture>> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool srgb);
    Ref<Mesh> ProcessAssimpMesh(const aiMesh* mesh, const aiScene* scene, const Mat4& transform);
    void ProcessAssimpNode(const aiNode* node, const aiScene* scene, const Mat4& parent_transform);
    void LoadModel(const std::string& path, const Transform& transform);

    std::string folder;
    std::vector<Ref<Mesh>> meshes;
    BVH bvh;
};

inline bool Model::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
    return bvh.Intersect(is, ray, t_min, t_max);
}

inline bool Model::GetAABB(AABB* out_aabb) const
{
    return bvh.GetAABB(out_aabb);
}

inline i32 Model::GetSize() const
{
    return bvh.GetSize();
}

inline size_t Model::GetMeshCount() const
{
    return meshes.size();
}

inline const std::vector<Ref<Mesh>>& Model::GetMeshes()
{
    return meshes;
}

} // namespace spt