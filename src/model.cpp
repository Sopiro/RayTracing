#include "raytracer/model.h"
#include "raytracer/triangle.h"

namespace spt
{

Model::Model(std::string_view path)
{
    LoadModel(path);

    // std::cout << "mesh count: " << meshes.size() << std::endl;

    for (int32 i = 0; i < meshes.size(); ++i)
    {
        Mesh* m = meshes[i].get();
        AABB aabb;
        m->GetAABB(aabb);

        bvh.CreateNode(m, aabb);
    }
}

bool Model::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
    return bvh.Hit(ray, t_min, t_max, rec);
}

bool Model::GetAABB(AABB& outAABB) const
{
    return bvh.GetAABB(outAABB);
}

std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<std::shared_ptr<Texture>> textures;

    for (uint32 i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::shared_ptr<Texture> texture = std::make_shared<ImageTexture>(folder + str.C_Str(), typeName);
        textures.push_back(texture);
    }

    return textures;
}

std::shared_ptr<Mesh> Model::ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, const Mat4& transform)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    // process meshes
    for (uint32 i = 0; i < mesh->mNumVertices; ++i)
    {
        Vec3 position = Vec3{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        Vec3 normal = Vec3{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        Vec2 texCoords{ 0.0f };

        if (mesh->HasTextureCoords(0))
        {
            texCoords.Set(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        vertices.emplace_back(position, normal, texCoords);
    }

    // process indices
    for (uint32 i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);

        for (uint32 j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");

        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    }

    return std::make_shared<Mesh>(vertices, indices, textures, transform);
}

void Model::ProcessAssimpNode(aiNode* node, const aiScene* scene, const Mat4& parent_transform)
{
    Mat4 transform = parent_transform * Convert(node->mTransformation);

    // process all the node's meshes (if any)
    for (uint32 i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessAssimpMesh(mesh, scene, transform));
    }

    // std::cout << node->mNumChildren << std::endl;

    // do the same for each of its children
    for (uint32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessAssimpNode(node->mChildren[i], scene, transform);
    }
}

void Model::LoadModel(std::string_view path)
{
    folder = path.substr(0, path.find_last_of('/'));
    folder.push_back('/');

    const aiScene* scene = importer.ReadFile(path.data(), aiProcessPreset_TargetRealtime_MaxQuality);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Assimp error:" << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessAssimpNode(scene->mRootNode, scene, Mat4{ identity });
}

} // namespace spt
