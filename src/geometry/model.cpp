#include "spt/model.h"
#include "spt/image_texture.h"
#include "spt/triangle.h"
#include "spt/util.h"

#include <filesystem>

namespace spt
{

Model::Model(const std::string& path, const Transform& transform)
{
    LoadModel(path, transform);
}

std::vector<Ref<Texture>> Model::LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool srgb)
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

Ref<Mesh> Model::ProcessAssimpMesh(const aiMesh* mesh, const aiScene* scene, const Mat4& transform)
{
    assert(mesh->HasPositions());
    assert(mesh->HasNormals());

    std::vector<Point3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec3> tangents;
    std::vector<UV> texCoords;

    uint32 vertexCount = mesh->mNumVertices;

    positions.resize(vertexCount);
    normals.resize(vertexCount);
    tangents.resize(vertexCount);
    texCoords.resize(vertexCount);

    // Process vertices
    for (uint32 i = 0; i < vertexCount; ++i)
    {
        positions[i].Set(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        normals[i].Set(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->HasTangentsAndBitangents())
        {
            tangents[i].Set(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        }
        else
        {
            tangents[i].SetZero();
        }

        if (mesh->HasTextureCoords(0))
        {
            texCoords[i].Set(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            texCoords[i].SetZero();
        }
    }

    // Process indices
    const int32 vertices_per_face = 3;

    std::vector<int32> indices;
    indices.reserve(mesh->mNumFaces * vertices_per_face);

    for (uint32 i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices == vertices_per_face)
        {
            for (uint32 j = 0; j < face.mNumIndices; ++j)
            {
                indices.emplace_back(face.mIndices[j]);
            }
        }
    }

    // Process materials
    MaterialColors colors;
    MaterialTextures textures;

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiColor3D diffuseColor;
        aiColor3D specularColor;
        aiColor3D emissiveColor;

        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);

        colors.diffuse.Set(diffuseColor.r, diffuseColor.g, diffuseColor.b);
        colors.specular.Set(specularColor.r, specularColor.g, specularColor.b);
        colors.emissive.Set(emissiveColor.r, emissiveColor.g, emissiveColor.b);

        auto basecolor_textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, true);
        auto metallic_textures = LoadMaterialTextures(material, aiTextureType_METALNESS, false);
        auto roughness_textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, false);
        auto emissive_textures = LoadMaterialTextures(material, aiTextureType_EMISSIVE, false);
        auto normal_map_textures = LoadMaterialTextures(material, aiTextureType_NORMALS, false);
        // auto ao_textures = LoadMaterialTextures(material, aiTextureType_LIGHTMAP, false);

        textures.basecolor = basecolor_textures.empty() ? nullptr : basecolor_textures[0];
        textures.normal_map = normal_map_textures.empty() ? nullptr : normal_map_textures[0];
        textures.metallic = metallic_textures.empty() ? nullptr : metallic_textures[0];
        textures.roughness = roughness_textures.empty() ? nullptr : roughness_textures[0];
        textures.emissive = emissive_textures.empty() ? nullptr : emissive_textures[0];
    }

    Ref<Material> material = CreateMaterial(textures, colors);

    return CreateSharedRef<Mesh>(std::move(positions), std::move(normals), std::move(tangents), std::move(texCoords),
                                 std::move(indices), transform, material);
}

void Model::ProcessAssimpNode(const aiNode* node, const aiScene* scene, const Mat4& parent_transform)
{
    Mat4 transform = Mul(parent_transform, Convert(node->mTransformation));

    // process all the node's meshes (if any)
    for (uint32 i = 0; i < node->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessAssimpMesh(mesh, scene, transform));
    }

    // do the same for each of its children
    for (uint32 i = 0; i < node->mNumChildren; ++i)
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
        aiProcessPreset_TargetRealtime_MaxQuality | 
        aiProcess_PreTransformVertices
    );
    // clang-format on

    if (scene == nullptr)
    {
        std::cout << "Faild to load model: " << path << std::endl;
        std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessAssimpNode(scene->mRootNode, scene, Mat4(transform));
}

} // namespace spt
