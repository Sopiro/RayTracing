#include "spt/model.h"
#include "spt/image_texture.h"
#include "spt/triangle.h"

#include <filesystem>

namespace spt
{

Model::Model(const std::string& path, const Transform& transform)
{
    LoadModel(path, transform);

    for (size_t i = 0; i < meshes.size(); ++i)
    {
        Mesh* m = meshes[i].get();
        AABB aabb;
        m->GetAABB(&aabb);

        bvh.CreateNode(m, aabb);
    }
}

std::vector<Ref<Texture>> Model::LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool srgb)
{
    std::vector<Ref<Texture>> textures;

    for (u32 i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // std::cout << str.C_Str() << std::endl;

        Ref<Texture> texture = ImageTexture::Create(folder + str.C_Str(), srgb);
        textures.push_back(texture);
    }

    return textures;
}

Ref<Mesh> Model::ProcessAssimpMesh(const aiMesh* mesh, const aiScene* scene, const Mat4& transform)
{
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);

    // Process vertices
    for (u32 i = 0; i < mesh->mNumVertices; ++i)
    {
        assert(mesh->HasPositions());
        assert(mesh->HasNormals());

        Vec3 position{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        Vec3 normal{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        Vec3 tangent{ 1.0, 0.0, 0.0 };
        if (mesh->HasTangentsAndBitangents())
        {
            tangent = Vec3{ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
        }

        Vec2 texCoord{ 0.0, 0.0 };
        if (mesh->HasTextureCoords(0))
        {
            texCoord.Set(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        vertices.emplace_back(position, normal, tangent, texCoord);
    }

    // Process indices
    const u32 vertices_per_face = 3;

    std::vector<u32> indices;
    indices.reserve(mesh->mNumFaces * vertices_per_face);

    for (u32 i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices == vertices_per_face)
        {
            for (u32 j = 0; j < face.mNumIndices; ++j)
            {
                indices.emplace_back(face.mIndices[j]);
            }
        }
    }

    // Process materials
    std::array<Ref<Texture>, TextureType::count> textures;

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

    // Transform vertex attributes
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        Vertex& v = vertices[i];

        Vec4 vP = transform * Vec4(v.position, 1.0);
        Vec4 vN = transform * Vec4(v.normal, 0.0);
        Vec4 vT = transform * Vec4(v.tangent, 0.0);
        vN.Normalize();
        vT.Normalize();

        v.position.Set(vP.x, vP.y, vP.z);
        v.normal.Set(vN.x, vN.y, vN.z);
        v.tangent.Set(vT.x, vT.y, vT.z);
    }

    return CreateSharedRef<Mesh>(vertices, indices, textures);
}

void Model::ProcessAssimpNode(const aiNode* node, const aiScene* scene, const Mat4& parent_transform)
{
    Mat4 transform = Mul(parent_transform, Convert(node->mTransformation));

    // process all the node's meshes (if any)
    for (u32 i = 0; i < node->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessAssimpMesh(mesh, scene, transform));
    }

    // do the same for each of its children
    for (u32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessAssimpNode(node->mChildren[i], scene, transform);
    }
}

void Model::LoadModel(const std::string& path, const Transform& transform)
{
    folder = std::move(std::filesystem::path(path).remove_filename().string());

    Assimp::Importer importer;

    // clang-format off
    const aiScene* scene = importer.ReadFile(
        path, 
        aiProcessPreset_TargetRealtime_Quality | 
        aiProcess_PreTransformVertices
    );
    // clang-format on

    if (scene == nullptr)
    {
        std::cout << "Faild to load model: " << path << std::endl;
        std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessAssimpNode(scene->mRootNode, scene, Mat4{ transform });
}

} // namespace spt
