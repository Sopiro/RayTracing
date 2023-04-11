#include "pathtracer/model.h"
#include "pathtracer/image_texture.h"
#include "pathtracer/triangle.h"

#include <filesystem>

namespace spt
{

Model::Model(std::string path, const Transform& transform)
{
    LoadModel(path, transform);

    for (size_t i = 0; i < meshes.size(); ++i)
    {
        Mesh* m = meshes[i].get();
        AABB aabb;
        m->GetAABB(aabb);

        bvh.CreateNode(m, aabb);
    }
}

std::vector<Ref<Texture>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, bool srgb)
{
    std::vector<Ref<Texture>> textures;

    for (uint32 i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // std::cout << str.C_Str() << std::endl;

        Ref<Texture> texture = ImageTexture::Create(folder + str.C_Str(), srgb);
        textures.push_back(texture);
    }

    return textures;
}

Ref<Mesh> Model::ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, const Mat4& transform)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    std::array<Ref<Texture>, TextureType::count> textures;

    // process vertices
    for (uint32 i = 0; i < mesh->mNumVertices; ++i)
    {
        Vec3 position{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        Vec3 normal{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        Vec3 tangent{ 1.0, 0.0, 0.0 };
        if (mesh->HasTangentsAndBitangents())
        {
            tangent = Vec3{ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
        }

        Vec2 texCoords{ 0.0, 0.0 };
        if (mesh->HasTextureCoords(0))
        {
            texCoords.Set(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        vertices.emplace_back(position, normal, tangent, texCoords);
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

    // process materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        auto basecolor_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, true);
        auto normal_maps = LoadMaterialTextures(material, aiTextureType_NORMALS, false);
        auto metallic_maps = LoadMaterialTextures(material, aiTextureType_METALNESS, false);
        auto roughness_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, false);
        auto ao_maps = LoadMaterialTextures(material, aiTextureType_LIGHTMAP, false);
        auto emissive_maps = LoadMaterialTextures(material, aiTextureType_EMISSIVE, false);

        textures[basecolor] = basecolor_maps.empty() ? nullptr : basecolor_maps[0];
        textures[normal] = normal_maps.empty() ? nullptr : normal_maps[0];
        textures[metallic] = metallic_maps.empty() ? nullptr : metallic_maps[0];
        textures[roughness] = roughness_maps.empty() ? nullptr : roughness_maps[0];
        textures[ao] = ao_maps.empty() ? nullptr : ao_maps[0];
        textures[emissive] = emissive_maps.empty() ? nullptr : emissive_maps[0];
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

    // do the same for each of its children
    for (uint32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessAssimpNode(node->mChildren[i], scene, transform);
    }
}

static Assimp::Importer importer;

void Model::LoadModel(std::string path, const Transform& transform)
{
    std::filesystem::path file_path(path);

    folder = file_path.parent_path().string();
    folder.push_back('/');

    const aiScene* scene = importer.ReadFile(path.data(), aiProcessPreset_TargetRealtime_MaxQuality);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Faild to load model: " << path << std::endl;
        std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessAssimpNode(scene->mRootNode, scene, Mat4{ transform });
}

} // namespace spt
