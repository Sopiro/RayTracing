#include "bulbit/model.h"
#include "bulbit/image_texture.h"
#include "bulbit/material.h"
#include "bulbit/triangle.h"
#include "bulbit/util.h"

#include <filesystem>

namespace bulbit
{

static Mat4 ConvertAssimpMatrix(const aiMatrix4x4& aiMat)
{
    Mat4 t;
    t.ex.Set(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1);
    t.ey.Set(aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2);
    t.ez.Set(aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3);
    t.ew.Set(aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);

    return t;
}

Model::Model(const std::string& filename, const Transform& transform)
{
    Load(filename, transform);
}

std::vector<Texture*> Model::LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool srgb)
{
    std::vector<Texture*> textures;

    for (uint32 i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // std::cout << str.C_Str() << std::endl;

        Texture* texture = ImageTexture::Create(folder + str.C_Str(), srgb);
        textures.push_back(texture);
    }

    return textures;
}

const Material* Model::CreateMaterial(const aiMesh* mesh, const aiScene* scene)
{
    assert(mesh->mMaterialIndex >= 0);

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    aiString name;
    ai_int illumModel;
    aiColor3D diffuseColor;
    aiColor3D specularColor;
    aiColor3D emissiveColor;
    ai_real metallic = 0;
    ai_real roughness = 1;
    ai_real aniso = 0;
    ai_real ior;

    material->Get(AI_MATKEY_NAME, name);
    material->Get(AI_MATKEY_SHADING_MODEL, illumModel);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);
    material->Get(AI_MATKEY_METALLIC_FACTOR, metallic);
    material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
    material->Get(AI_MATKEY_ANISOTROPY_FACTOR, aniso);
    material->Get(AI_MATKEY_REFRACTI, ior);

    auto basecolor_textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, true);
    auto metallic_textures = LoadMaterialTextures(material, aiTextureType_METALNESS, false);
    auto roughness_textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, false);
    auto emissive_textures = LoadMaterialTextures(material, aiTextureType_EMISSIVE, false);
    auto normalmap_textures = LoadMaterialTextures(material, aiTextureType_NORMALS, false);

    if (basecolor_textures.empty() && Material::fallback != nullptr)
    {
        return Material::fallback;
    }
    else
    {
        // clang-format off
        auto mat = std::make_unique<Microfacet>(
            basecolor_textures.empty() ? 
                ConstantColor::Create(diffuseColor.r, diffuseColor.g, diffuseColor.b)       : basecolor_textures[0],
            metallic_textures.empty() ? 
                ConstantColor::Create(metallic)                                             : metallic_textures[0],
            roughness_textures.empty() ? 
                ConstantColor::Create(roughness)                                            : roughness_textures[0],
            emissive_textures.empty() ? 
                ConstantColor::Create(emissiveColor.r, emissiveColor.g, emissiveColor.b)    : emissive_textures[0],
            normalmap_textures.empty() ? 
                ConstantColor::Create(0.5, 0.5, 1.0)                                        : normalmap_textures[0]
        );
        // clang-format on

        Material* ptr = mat.get();
        materials.push_back(std::move(mat));

        return ptr;
    }
}

std::shared_ptr<Mesh> Model::ProcessAssimpMesh(const aiMesh* mesh, const aiScene* scene, const Mat4& transform)
{
    assert(mesh->HasPositions());
    assert(mesh->HasNormals());

    std::vector<Point3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec3> tangents;
    std::vector<Point2> texCoords;

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

    return std::make_shared<Mesh>(std::move(positions), std::move(normals), std::move(tangents), std::move(texCoords),
                                  std::move(indices), CreateMaterial(mesh, scene), transform);
}

void Model::ProcessAssimpNode(const aiNode* node, const aiScene* scene, const Mat4& parent_transform)
{
    Mat4 transform = Mul(parent_transform, ConvertAssimpMatrix(node->mTransformation));

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

void Model::Load(const std::string& filename, const Transform& transform)
{
    folder = std::move(std::filesystem::path(filename).remove_filename().string());

    Assimp::Importer importer;

    // clang-format off
    const aiScene* scene = importer.ReadFile(
        filename, 
        aiProcessPreset_TargetRealtime_MaxQuality
    );
    // clang-format on

    if (scene == nullptr)
    {
        std::cout << "Faild to load model: " << filename << std::endl;
        std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessAssimpNode(scene->mRootNode, scene, Mat4(transform));
}

} // namespace bulbit
