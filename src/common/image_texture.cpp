#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "spt/image_texture_hdr.h"

namespace spt
{

Ref<ImageTexture> ImageTexture::Create(std::string path, bool srgb, bool hdr)
{
    auto loaded = loaded_textures.find(path);
    if (loaded != loaded_textures.end())
    {
        return loaded->second;
    }

    Ref<ImageTexture> image;

    if (hdr)
    {
        image = Ref<ImageTextureHDR>(new ImageTextureHDR(path, srgb));
    }
    else
    {
        image = Ref<ImageTexture>(new ImageTexture(path, srgb));
    }

    loaded_textures.emplace(path, image);
    ++texture_count;

    return image;
}

} // namespace spt
