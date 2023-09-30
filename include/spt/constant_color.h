#pragma once

#include "texture.h"

namespace spt
{

class ConstantColor : public Texture
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
    inline static std::unordered_map<Color, Ref<ConstantColor>, ColorHash> loaded_colors;

    static Ref<ConstantColor> Create(Color color);
    static Ref<ConstantColor> Create(Float rgb);
    static Ref<ConstantColor> Create(Float red, Float green, Float blue);

    virtual Color Value(const Point2& uv) const override;

private:
    ConstantColor() = default;
    ConstantColor(Color color);

    Color color;
};

inline ConstantColor::ConstantColor(Color _color)
    : color(_color)
{
}

inline Color ConstantColor::Value(const Point2& uv) const
{
    return color;
}

inline Ref<ConstantColor> ConstantColor::Create(Color color)
{
    auto loaded = loaded_colors.find(color);
    if (loaded != loaded_colors.end())
    {
        return loaded->second;
    }

    Ref<ConstantColor> ptr{ new ConstantColor(color) };
    loaded_colors.emplace(color, ptr);
    ++color_count;

    return ptr;
}

inline Ref<ConstantColor> ConstantColor::Create(Float rgb)
{
    return Create(Color(rgb));
}

inline Ref<ConstantColor> ConstantColor::Create(Float r, Float g, Float b)
{
    return Create(Color(r, g, b));
}

} // namespace spt
