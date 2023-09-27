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
            return std::hash<Float>()(v.x) ^ std::hash<Float>()(v.y) ^ std::hash<Float>()(v.z);
        }
    };

    inline static int32 color_count = 0;
    inline static std::unordered_map<Color, Ref<SolidColor>, ColorHash> loaded_colors;

    static Ref<SolidColor> Create(Color color);
    static Ref<SolidColor> Create(Float rgb);
    static Ref<SolidColor> Create(Float red, Float green, Float blue);

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

inline Ref<SolidColor> SolidColor::Create(Float rgb)
{
    return Create(Color(rgb));
}

inline Ref<SolidColor> SolidColor::Create(Float r, Float g, Float b)
{
    return Create(Color(r, g, b));
}

} // namespace spt
