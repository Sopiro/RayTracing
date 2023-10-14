#pragma once

#include "mesh.h"
#include "transform.h"

namespace spt
{

class Model
{
public:
    Model(const std::string& filename, const Transform& transform);
    virtual ~Model() = default;

    const std::vector<Ref<Mesh>>& GetMeshes();

private:
    friend class Scene;

    std::vector<Ref<Texture>> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool srgb);
    Ref<Mesh> ProcessAssimpMesh(const aiMesh* mesh, const aiScene* scene, const Mat4& transform);
    void ProcessAssimpNode(const aiNode* node, const aiScene* scene, const Mat4& parent_transform);
    void Load(const std::string& filename, const Transform& transform);

    std::string folder;
    std::vector<Ref<Mesh>> meshes;
};

inline const std::vector<Ref<Mesh>>& Model::GetMeshes()
{
    return meshes;
}

} // namespace spt