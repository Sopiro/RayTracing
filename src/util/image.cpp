#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include "bulbit/image.h"
#include "bulbit/parallel_for.h"
#include "bulbit/post_process.h"
#include "bulbit/spectrum.h"

namespace bulbit
{

Image1 ReadImage1(const std::filesystem::path& filename, int32 channel, bool non_color)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    float* data = stbi_loadf(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb_alpha);

    if (!data)
    {
        return {};
    }

    if (channel < 0 || channel >= components_per_pixel)
    {
        return {};
    }

    Image1 image(width, height);

    if (channel < 3)
    {
        if (width * height > 64 * 1024)
        {
            ParallelFor(0, width * height, [&](int32 i) { image[i] = Float(std::fmax(0, data[STBI_rgb_alpha * i + channel])); });
        }
        else
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = Float(std::fmax(0, data[STBI_rgb_alpha * i + channel]));
            }
        }
    }
    else if (components_per_pixel == STBI_rgb_alpha)
    {
        for (int32 i = 0; i < width * height; ++i)
        {
            image[i] = Float(std::fmax(0, data[STBI_rgb_alpha * i + channel]));
        }
    }

    stbi_image_free(data);
    return image;
}

Image3 ReadImage3(const std::filesystem::path& filename, bool non_color)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    float* data = stbi_loadf(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb);

    if (!data)
    {
        return {};
    }

    Image3 image(width, height);

    if (width * height > 64 * 1024)
    {
        ParallelFor(0, width * height, [&](int32 i) {
            image[i][0] = Float(std::fmax(0, data[STBI_rgb * i + 0]));
            image[i][1] = Float(std::fmax(0, data[STBI_rgb * i + 1]));
            image[i][2] = Float(std::fmax(0, data[STBI_rgb * i + 2]));
        });
    }
    else
    {
        for (int32 i = 0; i < width * height; ++i)
        {
            image[i][0] = Float(std::fmax(0, data[STBI_rgb * i + 0]));
            image[i][1] = Float(std::fmax(0, data[STBI_rgb * i + 1]));
            image[i][2] = Float(std::fmax(0, data[STBI_rgb * i + 2]));
        }
    }

    stbi_image_free(data);
    return image;
}

Image4 ReadImage4(const std::filesystem::path& filename, bool non_color)
{
    stbi_set_flip_vertically_on_load(true);
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    float* data = stbi_loadf(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb_alpha);

    if (!data)
    {
        return {};
    }

    Image4 image(width, height);

    if (width * height > 64 * 1024)
    {
        ParallelFor(0, width * height, [&](int32 i) {
            image[i][0] = Float(std::fmax(0, data[STBI_rgb_alpha * i + 0]));
            image[i][1] = Float(std::fmax(0, data[STBI_rgb_alpha * i + 1]));
            image[i][2] = Float(std::fmax(0, data[STBI_rgb_alpha * i + 2]));
        });
    }
    else
    {
        for (int32 i = 0; i < width * height; ++i)
        {
            image[i][0] = Float(std::fmax(0, data[STBI_rgb_alpha * i + 0]));
            image[i][1] = Float(std::fmax(0, data[STBI_rgb_alpha * i + 1]));
            image[i][2] = Float(std::fmax(0, data[STBI_rgb_alpha * i + 2]));
        }
    }

    if (components_per_pixel == STBI_rgb_alpha)
    {
        for (int32 i = 0; i < width * height; ++i)
        {
            image[i][3] = Float(std::fmax(0, data[STBI_rgb_alpha * i + 3]));
        }
    }

    stbi_image_free(data);
    return image;
}

void WriteImage(const Image3& image, const std::filesystem::path& filename)
{
    stbi_flip_vertically_on_write(true);

    std::string extension = filename.extension().string();
    if (extension == ".hdr")
    {
        stbi_write_hdr(filename.string().c_str(), image.width, image.height, 3, &image[0].r);
    }
    else if (extension == ".jpg")
    {
        std::vector<uint8> pixels(image.width * image.height * 3);

        for (int32 i = 0; i < image.width * image.height; ++i)
        {
            pixels[i * 3 + 0] = uint8(std::min(std::clamp(image[i][0], 0.0f, 1.0f) * 256.0, 255.0));
            pixels[i * 3 + 1] = uint8(std::min(std::clamp(image[i][1], 0.0f, 1.0f) * 256.0, 255.0));
            pixels[i * 3 + 2] = uint8(std::min(std::clamp(image[i][2], 0.0f, 1.0f) * 256.0, 255.0));
        }

        stbi_write_jpg(filename.string().c_str(), image.width, image.height, 3, &pixels[0], 100);
    }
    else if (extension == ".png")
    {
        std::vector<uint8> pixels(image.width * image.height * 3);

        for (int32 i = 0; i < image.width * image.height; ++i)
        {
            pixels[i * 3 + 0] = uint8(std::min(std::clamp(image[i][0], 0.0f, 1.0f) * 256.0, 255.0));
            pixels[i * 3 + 1] = uint8(std::min(std::clamp(image[i][1], 0.0f, 1.0f) * 256.0, 255.0));
            pixels[i * 3 + 2] = uint8(std::min(std::clamp(image[i][2], 0.0f, 1.0f) * 256.0, 255.0));
        }

        stbi_write_png(filename.string().c_str(), image.width, image.height, 3, &pixels[0], image.width * 3);
    }
    else
    {
        std::cout << "Faild to write image, type not supported: " << extension << std::endl;
    }
}

} // namespace bulbit
