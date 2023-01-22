#pragma once

#include "common.h"

class Texture
{
public:
    virtual Color Value(const UV& uv, const Vec3& p) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor() = default;
    SolidColor(Color _color)
        : color(_color)
    {
    }

    SolidColor(double red, double green, double blue)
        : SolidColor(Color(red, green, blue))
    {
    }

    virtual Color Value(const UV& uv, const Vec3& p) const override
    {
        return color;
    }

private:
    Color color;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture()
    {
    }

    CheckerTexture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd)
        : even{ _even }
        , odd{ _odd }
    {
    }

    CheckerTexture(Color c1, Color c2)
        : even{ std::make_shared<SolidColor>(c1) }
        , odd{ std::make_shared<SolidColor>(c2) }
    {
    }

    virtual Color Value(const UV& uv, const Vec3& p) const override
    {
        double sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);

        if (sines < 0)
        {
            return odd->Value(uv, p);
        }
        else
        {
            return even->Value(uv, p);
        }
    }

public:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

class ImageTexture : public Texture
{
public:
    const static int32 bytes_per_pixel = 3;

    ImageTexture()
        : data{ nullptr }
        , width{ 0 }
        , height{ 0 }
        , bytes_per_scanline{ 0 }
    {
    }

    ImageTexture(const char* filename)
    {
        int32 components_per_pixel = bytes_per_pixel;

        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data)
        {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~ImageTexture()
    {
        stbi_image_free(data);
    }

    virtual Color Value(const UV& uv, const Vec3& p) const override
    {
        if (data == nullptr)
        {
            return Color{ 1.0, 0.0, 1.0 };
        }

        // Clamp input texture coordinates to [0,1] x [1,0]
        double u = Clamp(uv.x, 0.0, 1.0);
        double v = 1.0 - Clamp(uv.y, 0.0, 1.0); // Flip V to image coordinates

        int32 i = static_cast<int32>(u * width);
        int32 j = static_cast<int32>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width)
        {
            i = width - 1;
        }
        if (j >= height)
        {
            j = height - 1;
        }

        double color_scale = 1.0 / 255.0;
        uint8* pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return Color{ color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2] };
    }

private:
    uint8* data;
    int32 width, height;
    int32 bytes_per_scanline;
};