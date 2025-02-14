#include "loader.h"
#include "scene_builder.h"

#define TINYOBJLOADER_IMPLEMENTATION
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

#include "material_builder.h"
#include "texture_builder.h"

namespace bulbit
{

static bool g_force_fallback_material = false;
static const Material* g_fallback_material = nullptr;
static MediumInterface g_fallback_medium_interface = {};
static bool g_flip_normal = false;
static bool g_flip_texcoord = false;

static std::string g_folder;
static std::vector<const Material*> g_materials;

void SetLoaderFlipNormal(bool flip_normal)
{
    g_flip_normal = flip_normal;
}

void SetLoaderFlipTexcoord(bool flip_texcoord)
{
    g_flip_texcoord = flip_texcoord;
}

void SetLoaderUseForceFallbackMaterial(bool force_use_fallback_material)
{
    g_force_fallback_material = force_use_fallback_material;
}

void SetLoaderFallbackMaterial(const Material* fallback_material)
{
    g_fallback_material = fallback_material;
}

void SetLoaderFallbackMediumInterface(const MediumInterface& fallback_medium_interface)
{
    g_fallback_medium_interface = fallback_medium_interface;
}

static void LoadMaterials(Scene& scene, tinygltf::Model& model)
{
    for (int32 i = 0; i < int32(model.materials.size()); i++)
    {
        tinygltf::Material& gltf_material = model.materials[i];
        tinygltf::PbrMetallicRoughness& pbr = gltf_material.pbrMetallicRoughness;

        SpectrumTexture *basecolor_texture, *normal_texture, *emission_texture;
        FloatTexture *metallic_texture, *roughness_texture, *alpha_texture;

        Spectrum basecolor_factor = { Float(pbr.baseColorFactor[0]), Float(pbr.baseColorFactor[1]),
                                      Float(pbr.baseColorFactor[2]) };
        Float metallic_factor = Float(pbr.metallicFactor);
        Float roughness_factor = Float(pbr.roughnessFactor);
        Spectrum emission_factor = { Float(gltf_material.emissiveFactor[0]), Float(gltf_material.emissiveFactor[1]),
                                     Float(gltf_material.emissiveFactor[2]) };

        // basecolor, alpha
        {
            if (pbr.baseColorTexture.index > -1)
            {
                tinygltf::Texture& texture = model.textures[pbr.baseColorTexture.index];
                tinygltf::Image& image = model.images[texture.source];

                basecolor_texture = CreateSpectrumImageTexture(scene, g_folder + image.uri);
                alpha_texture = CreateFloatImageTexture(scene, g_folder + image.uri, alpha_channel, true);
            }
            else
            {
                basecolor_texture = CreateSpectrumConstantTexture(scene, basecolor_factor);
                alpha_texture = nullptr;
            }
        }

        // metallic, roughness
        {
            if (pbr.metallicRoughnessTexture.index > -1)
            {
                tinygltf::Texture& texture = model.textures[pbr.metallicRoughnessTexture.index];
                tinygltf::Image& image = model.images[texture.source];

                metallic_texture = CreateFloatImageTexture(scene, g_folder + image.uri, metallic_channel, true);
                roughness_texture = CreateFloatImageTexture(scene, g_folder + image.uri, roughness_channel, true);
            }
            else
            {
                metallic_texture = CreateFloatConstantTexture(scene, metallic_factor);
                roughness_texture = CreateFloatConstantTexture(scene, roughness_factor);
            }
        }

        // normal
        {
            if (gltf_material.normalTexture.index > -1)
            {
                tinygltf::Texture& texture = model.textures[gltf_material.normalTexture.index];
                tinygltf::Image& image = model.images[texture.source];

                normal_texture = CreateSpectrumImageTexture(scene, g_folder + image.uri, true);
            }
            else
            {
                normal_texture = nullptr;
            }
        }

        // emission
        {
            if (gltf_material.emissiveTexture.index > -1)
            {
                tinygltf::Texture& texture = model.textures[gltf_material.emissiveTexture.index];
                tinygltf::Image& image = model.images[texture.source];

                emission_texture = CreateSpectrumImageTexture(scene, g_folder + image.uri);
            }
            else
            {
                emission_texture = CreateSpectrumConstantTexture(scene, emission_factor);
            }
        }

        g_materials.push_back(scene.CreateMaterial<UnrealMaterial>(
            basecolor_texture, metallic_texture, roughness_texture, roughness_texture, emission_texture, normal_texture,
            alpha_texture
        ));
    }
}

static bool LoadMesh(Scene& scene, tinygltf::Model& model, tinygltf::Mesh& mesh, const Mat4& transform)
{
    for (int32 prim = 0; prim < mesh.primitives.size(); ++prim)
    {
        tinygltf::Primitive& primitive = mesh.primitives[prim];

        if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
        {
            continue;
        }

        int32 position_index = primitive.attributes.contains("POSITION") ? primitive.attributes["POSITION"] : -1;
        int32 normal_index = primitive.attributes.contains("NORMAL") ? primitive.attributes["NORMAL"] : -1;
        int32 tangent_index = primitive.attributes.contains("TANGENT") ? primitive.attributes["TANGENT"] : -1;
        int32 texcoord_index = primitive.attributes.contains("TEXCOORD_0") ? primitive.attributes["TEXCOORD_0"] : -1;
        int32 indices_index = int32(primitive.indices);

        if (position_index == -1 || normal_index == -1 || texcoord_index == -1)
        {
            return false;
        }

        // position
        tinygltf::Accessor& position_accessor = model.accessors[position_index];
        tinygltf::BufferView& position_buffer_view = model.bufferViews[position_accessor.bufferView];
        tinygltf::Buffer& position_buffer = model.buffers[position_buffer_view.buffer];

        // normal
        tinygltf::Accessor& normal_accessor = model.accessors[normal_index];
        tinygltf::BufferView& normal_buffer_view = model.bufferViews[normal_accessor.bufferView];
        tinygltf::Buffer& normal_buffer = model.buffers[normal_buffer_view.buffer];

        // texcoord
        tinygltf::Accessor& texcoord_accessor = model.accessors[texcoord_index];
        tinygltf::BufferView& texcoord_buffer_view = model.bufferViews[texcoord_accessor.bufferView];
        tinygltf::Buffer& texcoord_buffer = model.buffers[texcoord_buffer_view.buffer];

        // indices
        tinygltf::Accessor& indices_accessor = model.accessors[indices_index];
        tinygltf::BufferView& indices_buffer_view = model.bufferViews[indices_accessor.bufferView];
        tinygltf::Buffer& indices_buffer = model.buffers[indices_buffer_view.buffer];

        std::vector<Vec3> positions(position_accessor.count);
        std::vector<Vec3> normals(normal_accessor.count);
        std::vector<Vec3> tangents(normal_accessor.count);
        std::vector<Vec2> texcoords(texcoord_accessor.count);
        std::vector<int32> indices(indices_accessor.count);

        size_t position_size = tinygltf::GetComponentSizeInBytes(position_accessor.componentType) *
                               tinygltf::GetNumComponentsInType(position_accessor.type);
        memcpy(
            positions.data(), position_buffer.data.data() + position_buffer_view.byteOffset + position_accessor.byteOffset,
            position_accessor.count * position_size
        );

        size_t normal_size = tinygltf::GetComponentSizeInBytes(normal_accessor.componentType) *
                             tinygltf::GetNumComponentsInType(normal_accessor.type);
        memcpy(
            normals.data(), normal_buffer.data.data() + normal_buffer_view.byteOffset + normal_accessor.byteOffset,
            normal_accessor.count * normal_size
        );

        size_t texcoord_size = tinygltf::GetComponentSizeInBytes(texcoord_accessor.componentType) *
                               tinygltf::GetNumComponentsInType(texcoord_accessor.type);
        memcpy(
            texcoords.data(), texcoord_buffer.data.data() + texcoord_buffer_view.byteOffset + texcoord_accessor.byteOffset,
            texcoord_accessor.count * texcoord_size
        );

        if (tangent_index != -1)
        {
            // tangent
            tinygltf::Accessor& tangent_accessor = model.accessors[tangent_index];
            tinygltf::BufferView& tangent_buffer_view = model.bufferViews[tangent_accessor.bufferView];
            tinygltf::Buffer& tangent_buffer = model.buffers[tangent_buffer_view.buffer];

            size_t tangent_size = tinygltf::GetNumComponentsInType(tangent_accessor.type) *
                                  tinygltf::GetComponentSizeInBytes(tangent_accessor.componentType);

            std::vector<Vec4> temp(tangent_accessor.count);
            memcpy(
                temp.data(), tangent_buffer.data.data() + tangent_buffer_view.byteOffset + tangent_accessor.byteOffset,
                tangent_accessor.count * tangent_size
            );

            for (size_t i = 0; i < tangent_accessor.count; ++i)
            {
                if (temp[i].w == 1)
                {
                    tangents[i].Set(temp[i].x, temp[i].y, temp[i].z);
                }
                else
                {
                    tangents[i].Set(-temp[i].x, -temp[i].y, -temp[i].z);
                }
            }
        }
        else
        {
            for (size_t i = 0; i < normals.size(); ++i)
            {
                Vec3 bitangent;
                CoordinateSystem(normals[i], &tangents[i], &bitangent);
            }
        }

        if (tinygltf::GetNumComponentsInType(indices_accessor.type) != 1)
        {
            return false;
        }

        size_t indices_size = tinygltf::GetComponentSizeInBytes(indices_accessor.componentType);
        switch (indices_size)
        {
        case sizeof(uint8_t):
        {
            std::vector<uint8_t> temp(indices_accessor.count);
            memcpy(
                temp.data(), indices_buffer.data.data() + indices_buffer_view.byteOffset + indices_accessor.byteOffset,
                indices_accessor.count * indices_size
            );
            for (size_t i = 0; i < indices_accessor.count; ++i)
            {
                indices[i] = int32_t(temp[i]);
            }
        }
        break;
        case sizeof(uint16_t):
        {
            std::vector<uint16_t> temp(indices_accessor.count);
            memcpy(
                temp.data(), indices_buffer.data.data() + indices_buffer_view.byteOffset + indices_accessor.byteOffset,
                indices_accessor.count * indices_size
            );
            for (size_t i = 0; i < indices_accessor.count; ++i)
            {
                indices[i] = int32_t(temp[i]);
            }
        }
        break;
        case sizeof(uint32_t):
        {
            std::vector<uint32_t> temp(indices_accessor.count);
            memcpy(
                temp.data(), indices_buffer.data.data() + indices_buffer_view.byteOffset + indices_accessor.byteOffset,
                indices_accessor.count * indices_size
            );
            for (size_t i = 0; i < indices_accessor.count; ++i)
            {
                indices[i] = int32_t(temp[i]);
            }
        }
        break;

        default:
            return false;
        }

        // post-processes
        {
            if (g_flip_normal)
            {
                for (size_t i = 0; i < normals.size(); ++i)
                {
                    normals[i].Negate();
                }
            }

            if (!g_flip_texcoord)
            {
                for (size_t i = 0; i < texcoords.size(); ++i)
                {
                    texcoords[i].y = -texcoords[i].y;
                }
            }
        }

        Mesh* m = scene.CreateMesh(
            std::move(positions), std::move(normals), std::move(tangents), std::move(texcoords), std::move(indices), transform
        );
        CreateTriangles(
            scene, m, g_force_fallback_material ? g_fallback_material : g_materials[primitive.material],
            g_fallback_medium_interface
        );
    }

    return true;
}

static void ProcessNode(Scene& my_scene, tinygltf::Model& model, tinygltf::Node& node, Mat4 parent)
{
    Mat4 transform =
        node.matrix.empty()
            ? identity
            : Mat4(
                  Vec4(float(node.matrix[0]), float(node.matrix[1]), float(node.matrix[2]), float(node.matrix[3])),
                  Vec4(float(node.matrix[4]), float(node.matrix[5]), float(node.matrix[6]), float(node.matrix[7])),
                  Vec4(float(node.matrix[8]), float(node.matrix[9]), float(node.matrix[10]), float(node.matrix[11])),
                  Vec4(float(node.matrix[12]), float(node.matrix[13]), float(node.matrix[14]), float(node.matrix[15]))
              );

    transform = Mul(parent, transform);

    if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
    {
        LoadMesh(my_scene, model, model.meshes[node.mesh], transform);
    }

    // Recursively process child nodes
    for (size_t i = 0; i < node.children.size(); i++)
    {
        BulbitAssert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
        ProcessNode(my_scene, model, model.nodes[node.children[i]], transform);
    }
}

static void LoadScene(Scene& my_scene, tinygltf::Model& model, const Transform& transform)
{
    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i)
    {
        BulbitAssert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
        ProcessNode(my_scene, model, model.nodes[scene.nodes[i]], Mat4(transform));
    }
}

void LoadGLTF(Scene& scene, std::filesystem::path filename, const Transform& transform)
{
    std::cout << "Loading.. " << filename.string() << std::endl;

    tinygltf::TinyGLTF gltf;
    gltf.SetImageLoader(
        [](tinygltf::Image* image, const int, std::string*, const std::string*, int, int, const unsigned char*, int,
           void*) -> bool {
            image->image.clear();
            image->width = 0;
            image->height = 0;
            image->component = 0;
            image->bits = 0;
            return true;
        },
        nullptr
    );
    tinygltf::Model model;

    std::string err, warn;

    bool success;
    if (filename.extension() == ".gltf")
    {
        success = gltf.LoadASCIIFromFile(&model, &err, &warn, filename.string());
    }
    else if (filename.extension() == ".glb")
    {
        success = gltf.LoadBinaryFromFile(&model, &err, &warn, filename.string());
    }
    else
    {
        std::cout << "Failed to load model: " << filename.string() << std::endl;
        return;
    }

    if (!success)
    {
        std::cout << "gltf warning: " << warn << std::endl;
        std::cout << "gltf error: " << err << std::endl;
        return;
    }

    g_folder = filename.remove_filename().string();
    if (!g_force_fallback_material)
    {
        LoadMaterials(scene, model);
    }
    LoadScene(scene, model, transform);
}

const Material* CreateOBJMaterial(Scene& scene, const tinyobj::material_t& mat, const std::string& root)
{
    Spectrum basecolor_factor = { Float(mat.diffuse[0]), Float(mat.diffuse[1]), Float(mat.diffuse[2]) };
    Float metallic_factor = 0;
    Float roughness_factor = 1;
    Spectrum emission_factor = { Float(mat.emission[0]), Float(mat.emission[1]), Float(mat.emission[2]) };

    // Create a texture for the diffuse component if available; otherwise use a constant texture.
    SpectrumTexture* basecolor_texture = nullptr;
    FloatTexture* alpha_texture = nullptr;
    if (!mat.diffuse_texname.empty())
    {
        basecolor_texture = CreateSpectrumImageTexture(scene, root + mat.diffuse_texname);
        alpha_texture = CreateFloatImageTexture(scene, root + mat.diffuse_texname, alpha_channel, true);
    }
    else
    {
        basecolor_texture = CreateSpectrumConstantTexture(scene, basecolor_factor);
    }

    // Use constant textures for metallic/roughness as OBJ does not provide them.
    FloatTexture* metallic_texture = CreateFloatConstantTexture(scene, metallic_factor);
    FloatTexture* roughness_texture = CreateFloatConstantTexture(scene, roughness_factor);

    // Use the bump texture as a normal texture if available.
    SpectrumTexture* normal_texture = nullptr;
    if (!mat.bump_texname.empty())
    {
        normal_texture = CreateSpectrumImageTexture(scene, root + mat.bump_texname, true);
    }

    // Create an emission texture if provided, otherwise use a constant emission texture.
    SpectrumTexture* emission_texture = nullptr;
    if (!mat.emissive_texname.empty())
    {
        emission_texture = CreateSpectrumImageTexture(scene, root + mat.emissive_texname);
    }
    else
    {
        emission_texture = CreateSpectrumConstantTexture(scene, emission_factor);
    }

    return scene.CreateMaterial<UnrealMaterial>(
        basecolor_texture, metallic_texture, roughness_texture, roughness_texture, emission_texture, normal_texture, alpha_texture
    );
}

// Structure to accumulate mesh data grouped by material.
struct OBJMeshGroup
{
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec3> tangents;
    std::vector<Vec2> texcoords;
    std::vector<int32> indices;
};

// Load OBJ model using tinyobj library.
void LoadOBJ(Scene& scene, std::filesystem::path filename, const Transform& transform)
{
    std::cout << "Loading OBJ: " << filename.string() << std::endl;

    // Extract folder path for textures and MTL file loading.
    g_folder = filename.parent_path().string();
    if (!g_folder.empty() && g_folder.back() != '/')
    {
        g_folder += "/";
    }

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = g_folder; // MTL file is assumed to be in the same folder as the OBJ.

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename.string(), reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader Error: " << reader.Error() << std::endl;
        }
        return;
    }
    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader Warning: " << reader.Warning() << std::endl;
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();
    const auto& materials = reader.GetMaterials();

    // Convert OBJ materials to engine Materials. The index corresponds to the material ID.
    std::vector<const Material*> obj_materials(materials.size());
    for (size_t i = 0; i < materials.size(); i++)
    {
        obj_materials[i] = (!g_force_fallback_material) ? CreateOBJMaterial(scene, materials[i], g_folder) : g_fallback_material;
    }

    // Process each shape in the OBJ file.
    for (const auto& shape : shapes)
    {
        size_t index_offset = 0;
        // Group faces by material ID (a shape may mix different materials).
        std::unordered_map<int, OBJMeshGroup> groups;
        groups.reserve(4); // Reserve some typical material group count.

        // Iterate over each face.
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
        {
            int material_id = shape.mesh.material_ids[f]; // Material ID for this face (-1 if none)
            OBJMeshGroup& group = groups[material_id];
            size_t fv = size_t(shape.mesh.num_face_vertices[f]);

            std::vector<int> face_indices;
            face_indices.reserve(fv); // Reserve space for vertex indices in the face

            // Process each vertex of the face.
            for (size_t v = 0; v < fv; v++)
            {
                const tinyobj::index_t& idx = shape.mesh.indices[index_offset + v];

                // Retrieve vertex position.
                Vec3 position;
                position.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                position.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                position.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                // Retrieve normal if available.
                Vec3 normal(0);
                if (idx.normal_index >= 0)
                {
                    normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
                }

                // Retrieve texture coordinates if available.
                Vec2 texcoord(0);
                if (idx.texcoord_index >= 0)
                {
                    texcoord.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    texcoord.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }

                // Calculate tangent from the normal (since OBJ usually doesn't provide tangents).
                Vec3 tangent(0);
                if (normal != Vec3::zero)
                {
                    Vec3 bitangent;
                    CoordinateSystem(normal, &tangent, &bitangent);
                }

                group.positions.push_back(position);
                group.normals.push_back(normal);
                group.tangents.push_back(tangent);
                group.texcoords.push_back(texcoord);
                face_indices.push_back(int32(group.positions.size() - 1));
            }

            // Triangulate the face using a triangle fan approach if necessary.
            if (fv >= 3)
            {
                if (fv == 3)
                {
                    group.indices.insert(group.indices.end(), face_indices.begin(), face_indices.end());
                }
                else
                {
                    for (size_t v = 1; v < fv - 1; v++)
                    {
                        group.indices.push_back(face_indices[0]);
                        group.indices.push_back(face_indices[v]);
                        group.indices.push_back(face_indices[v + 1]);
                    }
                }
            }
            index_offset += fv;
        }

        // Create a mesh for each material group and add it to the scene.
        for (auto& [material_id, group] : groups)
        {
            Mesh* m = scene.CreateMesh(
                std::move(group.positions), std::move(group.normals), std::move(group.tangents), std::move(group.texcoords),
                std::move(group.indices), transform
            );

            const Material* material = (material_id < 0 || size_t(material_id) >= obj_materials.size())
                                           ? g_fallback_material
                                           : obj_materials[material_id];

            CreateTriangles(scene, m, g_force_fallback_material ? g_fallback_material : material, g_fallback_medium_interface);
        }
    }
}

void LoadModel(Scene& scene, std::filesystem::path filename, const Transform& transform)
{
    auto ext = filename.extension();
    if (ext == ".gltf" || ext == ".glb")
    {
        LoadGLTF(scene, filename, transform);
    }
    else if (filename.extension() == ".obj")
    {
        LoadOBJ(scene, filename, transform);
    }
    else
    {
        std::cout << "Failed to load model: " << filename.string() << std::endl;
        std::cout << "Supported extensions: .obj .gltf" << std::endl;
    }
}

} // namespace bulbit
