#pragma once

#include "texture.h"

namespace spt
{

class SolidColor : public Texture
{
public:
    struct ColorHash
    {
        size_t operator()(const Color& v) const
        {
            return std::hash<f64>()(v.x) ^ std::hash<f64>()(v.y) ^ std::hash<f64>()(v.z);
        }
    };

    inline static i32 color_count = 0;
    inline static std::unordered_map<Color, Ref<SolidColor>, ColorHash> loaded_colors;

    static Ref<SolidColor> Create(Color color);
    static Ref<SolidColor> Create(f64 rgb);
    static Ref<SolidColor> Create(f64 red, f64 green, f64 blue);

    virtual Color Value(const UV& uv) const override;

private:
    SolidColor() = default;
    SolidColor(Color color);

    Color color;
};

inline SolidColor::SolidColor(Color _color)
    : color(_color)
{
}

inline Color SolidColor::Value(const UV& uv) const
{
    return color;
}

inline Ref<SolidColor> SolidColor::Create(Color color)
{
    auto loaded = loaded_colors.find(color);
    if (loaded != loaded_colors.end())
    {
        return loaded->second;
    }

    Ref<SolidColor> ptr{ new SolidColor(color) };
    loaded_colors.emplace(color, ptr);
    ++color_count;

    return ptr;
}

inline Ref<SolidColor> SolidColor::Create(f64 rgb)
{
    return Create(Color(rgb));
}

inline Ref<SolidColor> SolidColor::Create(f64 r, f64 g, f64 b)
{
    return Create(Color(r, g, b));
}

} // namespace spt
