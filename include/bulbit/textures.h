#pragma once

#include "hash.h"
#include "image.h"
#include "pool.h"
#include "texture.h"

namespace bulbit
{

template <typename T>
class ConstantTexture : public Texture<T>
{
public:
    ConstantTexture(T value)
        : value{ std::move(value) }
    {
    }

    T Evaluate(const Point2& uv) const
    {
        BulbitNotUsed(uv);
        return value;
    }

private:
    T value;
};

template <typename T>
class ImageTexture : public Texture<T>
{
public:
    ImageTexture(Image<T> image, TexCoordFilter texcoord_filter)
        : image{ std::move(image) }
        , texcoord_filter{ texcoord_filter }
    {
    }

    int32 GetWidth() const
    {
        return image.width;
    }

    int32 GetHeight() const
    {
        return image.height;
    }

    T Evaluate(const Point2& uv) const
    {
#if 0
        // Nearest sampling
        Float w = uv.x * image.width + 0.5f;
        Float h = uv.y * image.height + 0.5f;

        int32 i = int32(w);
        int32 j = int32(h);

        FilterTexCoord(&i, &j);

        return image[i + j * image.width];
#else
        // Bilinear sampling
        Float w = uv.x * image.width + 0.5f;
        Float h = uv.y * image.height + 0.5f;

        int32 i0 = int32(w), i1 = int32(w) + 1;
        int32 j0 = int32(h), j1 = int32(h) + 1;

        FilterTexCoord(&i0, &j0, image.width, image.height, texcoord_filter);
        FilterTexCoord(&i1, &j1, image.width, image.height, texcoord_filter);

        Float fu = w - std::floor(w);
        Float fv = h - std::floor(h);

        T v00 = image[i0 + j0 * image.width], v10 = image[i1 + j0 * image.width];
        T v01 = image[i0 + j1 * image.width], v11 = image[i1 + j1 * image.width];

        // clang-format off
    return (1-fu) * (1-fv) * v00 + (1-fu) * (fv) * v01 +
           (  fu) * (1-fv) * v10 + (  fu) * (fv) * v11;
        // clang-format on
#endif
    }

private:
    Image<T> image;
    TexCoordFilter texcoord_filter;
};

template <typename T>
class CheckerTexture : public Texture<T>
{
public:
    CheckerTexture(const Texture<T>* a, const Texture<T>* b, const Point2& resolution)
        : a{ a }
        , b{ b }
        , resolution{ resolution }
    {
    }

    virtual T Evaluate(const Point2& uv) const override
    {
        Point2i scale = uv * resolution;
        int32 c = scale.x + scale.y;

        if (c % 2)
        {
            return a->Evaluate(uv);
        }
        else
        {
            return b->Evaluate(uv);
        }
    }

private:
    const Texture<T>* a;
    const Texture<T>* b;
    Point2 resolution;
};

using FloatConstantTexture = ConstantTexture<Float>;
using SpectrumConstantTexture = ConstantTexture<Spectrum>;

using FloatImageTexture = ImageTexture<Float>;
using SpectrumImageTexture = ImageTexture<Spectrum>;

using FloatCheckerTexture = CheckerTexture<Float>;
using SpectrumCheckerTexture = CheckerTexture<Spectrum>;

namespace detail
{

using key_2d1f = std::tuple<std::string, int32, bool>;
using key_2d3f = std::tuple<std::string, bool>;
using key_c1f = std::tuple<const Texture<Float>*, const Texture<Float>*, Point2>;
using key_c3f = std::tuple<const Texture<Spectrum>*, const Texture<Spectrum>*, Point2>;

struct hasher_1d3f
{
    size_t operator()(const Spectrum& key) const
    {
        return Hash(key);
    }
};

struct hasher_2d1f
{
    size_t operator()(const key_2d1f& key) const
    {
        const std::string& filename = std::get<0>(key);
        int32 channel = std::get<1>(key);
        bool is_non_color = std::get<2>(key);

        size_t string_hash = HashBuffer(filename.data(), filename.size());
        return Hash(string_hash, channel, is_non_color);
    }
};

struct hasher_2d3f
{
    size_t operator()(const key_2d3f& key) const
    {
        const std::string& filename = std::get<0>(key);
        bool is_non_color = std::get<1>(key);

        size_t string_hash = HashBuffer(filename.data(), filename.size());
        return Hash(string_hash, is_non_color);
    }
};

struct hasher_c1f
{
    size_t operator()(const key_c1f& key) const
    {
        const FloatTexture* a = std::get<0>(key);
        const FloatTexture* b = std::get<1>(key);
        Point2 resolution = std::get<2>(key);

        return Hash(a, b, resolution);
    }
};

struct hasher_c3f
{
    size_t operator()(const key_c3f& key) const
    {
        const SpectrumTexture* a = std::get<0>(key);
        const SpectrumTexture* b = std::get<1>(key);
        Point2 resolution = std::get<2>(key);

        return Hash(a, b, resolution);
    }
};

}; // namespace detail

struct TexturePool
{
    Pool<Float, ConstantTexture<Float>> pool_1f;
    Pool<Spectrum, ConstantTexture<Spectrum>, detail::hasher_1d3f> pool_3f;

    Pool<detail::key_2d1f, ImageTexture<Float>, detail::hasher_2d1f> pool_2d1f;
    Pool<detail::key_2d3f, ImageTexture<Spectrum>, detail::hasher_2d3f> pool_2d3f;

    Pool<detail::key_c1f, CheckerTexture<Float>, detail::hasher_c1f> pool_c1f;
    Pool<detail::key_c3f, CheckerTexture<Spectrum>, detail::hasher_c3f> pool_c3f;
};

inline TexturePool texture_pool;

FloatConstantTexture* CreateFloatConstantTexture(Float value);
SpectrumConstantTexture* CreateSpectrumConstantTexture(const Spectrum& value);
SpectrumConstantTexture* CreateSpectrumConstantTexture(Float value);
SpectrumConstantTexture* CreateSpectrumConstantTexture(Float r, Float g, Float b);

FloatImageTexture* CreateFloatImageTexture(std::string filename, int32 channel, bool is_non_color = false);
SpectrumImageTexture* CreateSpectrumImageTexture(std::string filename, bool is_non_color = false);

FloatCheckerTexture* CreateFloatCheckerTexture(const FloatTexture* a, const FloatTexture* b, const Point2& resolution);
SpectrumCheckerTexture* CreateSpectrumCheckerTexture(
    const SpectrumTexture* a, const SpectrumTexture* b, const Point2& resolution
);

} // namespace bulbit
