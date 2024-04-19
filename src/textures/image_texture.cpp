#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "bulbit/image_texture.h"

#include "bulbit/parallel_for.h"

namespace bulbit
{

ImageTexture* ImageTexture::Create(const std::string& filename, bool srgb)
{
    auto loaded = loaded_textures.find(filename);
    if (loaded != loaded_textures.end())
    {
        return loaded->second.get();
    }

    std::unique_ptr<ImageTexture> image = std::make_unique<ImageTexture>(filename, srgb);
    ImageTexture* ptr = image.get();

    loaded_textures.emplace(filename, std::move(image));
    ++texture_count;

    return ptr;
}

ImageTexture::ImageTexture()
    : pixels{ nullptr }
    , width{ 0 }
    , height{ 0 }
    , texcoord_filter{ repeat }
{
}

ImageTexture::ImageTexture(const std::string& filename, bool srgb)
    : texcoord_filter{ repeat }
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_scale(1.0f);
    stbi_ldr_to_hdr_gamma(srgb ? 2.2f : 1.0f);

    assert(STBI_rgb == 3);

    int32 components_per_pixel;
    float* data = stbi_loadf(filename.data(), &width, &height, &components_per_pixel, STBI_rgb);

    if (!data)
    {
        std::cerr << "ERROR: Could not load texture '" << filename << std::endl;
        width = 1;
        height = 1;
        pixels = std::make_unique<Spectrum[]>(width * height);

        pixels[0] = Spectrum(1, 0, 1);
    }
    else
    {
        pixels = std::make_unique<Spectrum[]>(width * height);

        ParallelFor(0, width * height, [=](int32 i) {
            pixels[i].r = (Float)std::fmax(0, data[STBI_rgb * i + 0]);
            pixels[i].g = (Float)std::fmax(0, data[STBI_rgb * i + 1]);
            pixels[i].b = (Float)std::fmax(0, data[STBI_rgb * i + 2]);
        });

        stbi_image_free(data);
    }
}

Spectrum ImageTexture::Evaluate(const Point2& uv) const
{
#if 0
    // Nearest sampling
    Float w = uv.x * width + Float(0.5);
    Float h = uv.y * height + Float(0.5);

    int32 i = int32(w);
    int32 j = int32(h);

    FilterTexCoord(&i, &j);

    return pixels[i + j * width];
#else
    // Bilinear sampling
    Float w = uv.x * width + Float(0.5);
    Float h = uv.y * height + Float(0.5);

    int32 i0 = int32(w), i1 = int32(w) + 1;
    int32 j0 = int32(h), j1 = int32(h) + 1;

    FilterTexCoord(&i0, &j0);
    FilterTexCoord(&i1, &j1);

    Float fu = w - std::floor(w);
    Float fv = h - std::floor(h);

    Spectrum sp00 = pixels[i0 + j0 * width], sp10 = pixels[i1 + j0 * width];
    Spectrum sp01 = pixels[i0 + j1 * width], sp11 = pixels[i1 + j1 * width];

    // clang-format off
    return (1-fu) * (1-fv) * sp00 + (1-fu) * (fv) * sp01 +
           (  fu) * (1-fv) * sp10 + (  fu) * (fv) * sp11;
    // clang-format on
#endif
}

void ImageTexture::FilterTexCoord(int32* u, int32* v) const
{
    switch (texcoord_filter)
    {
    case TexCoordFilter::repeat:
    {
        *u = *u >= 0 ? *u % width : width - (-(*u) % width) - 1;
        *v = *v >= 0 ? *v % height : height - (-(*v) % height) - 1;
    }
    break;
    case TexCoordFilter::clamp:
    {
        *u = Clamp(*u, 0, width - 1);
        *v = Clamp(*v, 0, height - 1);
    }
    break;
    default:
        assert(false);
        break;
    }
}

} // namespace bulbit
