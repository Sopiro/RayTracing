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
    SolidColor()
    {
    }
    SolidColor(Color c)
        : color_value(c)
    {
    }

    SolidColor(double red, double green, double blue)
        : SolidColor(Color(red, green, blue))
    {
    }

    virtual Color Value(const UV& uv, const Vec3& p) const override
    {
        return color_value;
    }

private:
    Color color_value;
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