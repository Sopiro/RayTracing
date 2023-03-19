#pragma once

#include "common.h"
#include "mesh.h"
#include "transform.h"

namespace spt
{

static Assimp::Importer importer;

class Model : public Hittable
{
public:
    Model(std::string_view path, const Transform& transform);

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

private:
    std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    std::shared_ptr<Mesh> ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, const Mat4& transform);
    void ProcessAssimpNode(aiNode* node, const aiScene* scene, const Mat4& parent_transform);
    void LoadModel(std::string_view path, const Transform& transform);

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

} // namespace spt