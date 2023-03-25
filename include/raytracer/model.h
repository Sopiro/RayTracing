#pragma once

#include "common.h"
#include "mesh.h"
#include "transform.h"

namespace spt
{

class Model : public Hittable
{
public:
    Model(std::string path, const Transform& transform);

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

    size_t GetMeshCount() const;
    std::vector<std::shared_ptr<Mesh>> GetMeshes();

private:
    std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);
    std::shared_ptr<Mesh> ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, const Mat4& transform);
    void ProcessAssimpNode(aiNode* node, const aiScene* scene, const Mat4& parent_transform);
    void LoadModel(std::string path, const Transform& transform);

    std::string folder;
    std::vector<std::shared_ptr<Mesh>> meshes;
    BVH bvh;
};

inline bool Model::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
    return bvh.Hit(ray, t_min, t_max, rec);
}

inline bool Model::GetAABB(AABB& outAABB) const
{
    return bvh.GetAABB(outAABB);
}

inline size_t Model::GetMeshCount() const
{
    return meshes.size();
}

inline std::vector<std::shared_ptr<Mesh>> Model::GetMeshes()
{
    return meshes;
}

} // namespace spt