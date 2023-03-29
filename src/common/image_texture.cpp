#include "raytracer/image_texture_hdr.h"

namespace spt
{

std::shared_ptr<ImageTexture> ImageTexture::Create(std::string path, bool srgb, bool hdr)
{
    auto loaded = loaded_textures.find(path);
    if (loaded != loaded_textures.end())
    {
        return loaded->second;
    }

    std::shared_ptr<ImageTexture> image;

    if (hdr)
    {
        image = std::shared_ptr<ImageTextureHDR>(new ImageTextureHDR(path, srgb));
    }
    else
    {
        image = std::shared_ptr<ImageTexture>(new ImageTexture(path, srgb));
    }

    loaded_textures.emplace(path, image);
    ++texture_count;

    return image;
}

} // namespace spt
