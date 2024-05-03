#include "bulbit/loader.h"
#include "bulbit/mesh.h"
#include "bulbit/scene_builder.h"
#include "bulbit/triangle.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <filesystem>

namespace bulbit
{

static std::string g_folder;
static Scene* g_scene = nullptr;
static const Material* g_fallback_material = nullptr;

Mat4 ConvertAssimpMatrix(const aiMatrix4x4& aiMat)
{
    Mat4 t;
    t.ex.Set(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1);
    t.ey.Set(aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2);
    t.ez.Set(aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3);
    t.ew.Set(aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);

    return t;
}

static std::vector<SpectrumTexture*> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool srgb)
{
    std::vector<SpectrumTexture*> textures;
    textures.reserve(mat->GetTextureCount(type));

    for (uint32 i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // std::cout << str.C_Str() << std::endl;

        SpectrumTexture* texture = ImageTexture::Create(g_folder + str.C_Str(), srgb);
        textures.push_back(texture);
    }

    return textures;
}

static std::vector<FloatTexture*> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, int32 channel, bool srgb)
{
    std::vector<FloatTexture*> textures;
    textures.reserve(mat->GetTextureCount(type));

    for (uint32 i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // std::cout << str.C_Str() << std::endl;

        FloatTexture* texture = FloatImageTexture::Create(g_folder + str.C_Str(), channel, srgb);
        textures.push_back(texture);
    }

    return textures;
}

static const Material* LoadMaterial(const aiMesh* mesh, const aiScene* scene)
{
    assert(mesh->mMaterialIndex >= 0);

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    aiString name;
    ai_int illum_model;
    aiColor3D diffuse_color;
    aiColor3D specular_color;
    aiColor3D emissive_color;
    ai_real metallic = 0;
    ai_real roughness = 1;
    ai_real aniso = 0;
    ai_real ior;

    material->Get(AI_MATKEY_NAME, name);
    material->Get(AI_MATKEY_SHADING_MODEL, illum_model);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive_color);
    material->Get(AI_MATKEY_METALLIC_FACTOR, metallic);
    material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
    material->Get(AI_MATKEY_ANISOTROPY_FACTOR, aniso);
    material->Get(AI_MATKEY_REFRACTI, ior);

    std::vector<SpectrumTexture*> basecolor_textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, true);
    std::vector<FloatTexture*> metallic_textures =
        LoadMaterialTextures(material, aiTextureType_METALNESS, metallic_channel, false);
    std::vector<FloatTexture*> roughness_textures =
        LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, roughness_channel, false);
    std::vector<SpectrumTexture*> emissive_textures = LoadMaterialTextures(material, aiTextureType_EMISSIVE, false);
    std::vector<SpectrumTexture*> normalmap_textures = LoadMaterialTextures(material, aiTextureType_NORMALS, false);

    if (basecolor_textures.empty() && g_fallback_material != nullptr)
    {
        return g_fallback_material;
    }

    // Create material from scene

    // clang-format off
    auto mat = g_scene->CreateMaterial<Microfacet>(
        basecolor_textures.empty() ? 
            ConstantColor::Create(diffuse_color.r, diffuse_color.g, diffuse_color.b)    : basecolor_textures[0],
        metallic_textures.empty() ? 
            ConstantFloatTexture::Create(metallic)                                      : metallic_textures[0],
        roughness_textures.empty() ? 
            ConstantFloatTexture::Create(roughness)                                     : roughness_textures[0],
        emissive_textures.empty() ? 
            ConstantColor::Create(emissive_color.r, emissive_color.g, emissive_color.b) : emissive_textures[0],
        normalmap_textures.empty() ? 
            ConstantColor::Create(0.5, 0.5, 1.0)                                        : normalmap_textures[0]
    );
    // clang-format on

    return mat;
}

static std::unique_ptr<Mesh> ProcessAssimpMesh(const aiMesh* mesh, const aiScene* scene, const Mat4& transform)
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

    return std::make_unique<Mesh>(std::move(positions), std::move(normals), std::move(tangents), std::move(texCoords),
                                  std::move(indices), LoadMaterial(mesh, scene), transform);
}

static void ProcessAssimpNode(const aiNode* node, const aiScene* scene, const Mat4& parent_transform)
{
    Mat4 transform = Mul(parent_transform, ConvertAssimpMatrix(node->mTransformation));

    // Process all the node's meshes
    for (uint32 i = 0; i < node->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        CreateTriangles(*g_scene, ProcessAssimpMesh(mesh, scene, transform));
    }

    // Do the same for each of its children
    for (uint32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessAssimpNode(node->mChildren[i], scene, transform);
    }
}

void LoadModel(Scene& scene, const std::string& filename, const Transform& transform, const Material* fallback_material)
{
    g_folder = std::filesystem::path(filename).remove_filename().string();
    g_scene = &scene;
    g_fallback_material = fallback_material;

    Assimp::Importer importer;

    // clang-format off
    const aiScene* assimp_scene = importer.ReadFile(
        filename, 
        aiProcessPreset_TargetRealtime_MaxQuality
    );
    // clang-format on

    if (assimp_scene == nullptr)
    {
        std::cout << "Faild to load model: " << filename << std::endl;
        std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessAssimpNode(assimp_scene->mRootNode, assimp_scene, Mat4(transform));
}

} // namespace bulbit
