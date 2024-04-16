#pragma once

#include "mesh.h"
#include "transform.h"

namespace bulbit
{

class Texture;
class Material;

// simply a collection of meshes
class Model
{
public:
    Model(const std::string& filename, const Transform& transform);
    virtual ~Model() = default;

    const std::vector<Ref<Mesh>>& GetMeshes() const;

private:
    friend class Scene;

    std::vector<Ref<Texture>> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool srgb);
    const Material* CreateMaterial(const aiMesh* mesh, const aiScene* scene);
    Ref<Mesh> ProcessAssimpMesh(const aiMesh* mesh, const aiScene* scene, const Mat4& transform);
    void ProcessAssimpNode(const aiNode* node, const aiScene* scene, const Mat4& parent_transform);
    void Load(const std::string& filename, const Transform& transform);

    std::string folder;

    std::vector<Ref<Mesh>> meshes;
    std::vector<Ref<Material>> materials;
};

inline const std::vector<Ref<Mesh>>& Model::GetMeshes() const
{
    return meshes;
}

} // namespace bulbit