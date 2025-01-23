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

template <typename T>
using key_0d = T;

template <typename T>
using key_2d = std::pair<const T*, int32>;

template <typename T>
using key_c = std::tuple<const Texture<T>*, const Texture<T>*, Point2>;

template <typename T>
struct hasher_0d
{
    size_t operator()(key_0d<T> key) const
    {
        return Hash(key);
    }
};

template <typename T>
struct hasher_2d
{
    size_t operator()(key_2d<T> image) const
    {
        const T* buffer = image.first;
        int32 size = image.second;

        return HashBuffer(buffer, size);
    }
};

template <typename T>
struct hasher_c
{
    size_t operator()(key_c<T> key) const
    {
        const Texture<T>* a = std::get<0>(key);
        const Texture<T>* b = std::get<1>(key);
        Point2 resolution = std::get<2>(key);

        return Hash(a, b, resolution);
    }
};

template <typename T>
using Pool0d = Pool<detail::key_0d<T>, ConstantTexture<T>, detail::hasher_0d<T>>;

template <typename T>
using Pool2d = Pool<detail::key_2d<T>, ImageTexture<T>, detail::hasher_2d<T>>;

template <typename T>
using PoolC = Pool<detail::key_c<T>, CheckerTexture<T>, detail::hasher_c<T>>;

}; // namespace detail

class TexturePool
{
public:
    template <template <typename> class TextureType, typename T, typename... Args>
    TextureType<T>* CreateTexture(Args&&... args)
    {
        if constexpr (std::is_same_v<TextureType<T>, ConstantTexture<T>>)
        {
            // For ConstantTexture
            if constexpr (sizeof...(Args) == 1)
            {
                T value = std::get<0>(std::forward_as_tuple(args...));
                return GetPool<TextureType, T>().Create(value, value);
            }
            else
            {
                throw std::invalid_argument("Invalid arguments for ConstantTexture");
            }
        }
        else if constexpr (std::is_same_v<TextureType<T>, ImageTexture<T>>)
        {
            // For ImageTexture
            if constexpr (sizeof...(Args) == 1)
            {
                auto& image = std::get<0>(std::forward_as_tuple(args...));
                if (image)
                {
                    return GetPool<TextureType, T>().Create(
                        { &image[0], image.width * image.height }, std::move(image), TexCoordFilter::repeat
                    );
                }
                else
                {
                    return nullptr;
                }
            }
            else
            {
                throw std::invalid_argument("Invalid arguments for ImageTexture");
            }
        }
        else if constexpr (std::is_same_v<TextureType<T>, CheckerTexture<T>>)
        {
            // For CheckerTexture
            if constexpr (sizeof...(Args) == 3)
            {
                using FirstArg = std::decay_t<decltype(std::get<0>(std::forward_as_tuple(args...)))>;
                using SecondArg = std::decay_t<decltype(std::get<1>(std::forward_as_tuple(args...)))>;
                using ThirdArg = std::decay_t<decltype(std::get<2>(std::forward_as_tuple(args...)))>;

                if constexpr (std::is_same_v<FirstArg, const Texture<T>*> && std::is_same_v<SecondArg, const Texture<T>*> &&
                              std::is_same_v<ThirdArg, Point2>)
                {
                    return GetPool<TextureType, T>().Create(std::forward<Args>(args)...);
                }
                else
                {
                    throw std::invalid_argument("Invalid arguments for CheckerTexture");
                }
            }
            else
            {
                throw std::invalid_argument("Invalid arguments for CheckerTexture");
            }
        }
        else
        {
            throw std::invalid_argument("Unsupported texture type");
        }
    }

private:
    template <template <typename> class TextureType, typename T>
    auto& GetPool()
    {
        if constexpr (std::is_same_v<TextureType<T>, ConstantTexture<T>>)
        {
            if constexpr (std::is_same_v<T, Float>)
            {
                return pool_0d1f;
            }
            else if constexpr (std::is_same_v<T, Spectrum>)
            {
                return pool_0d3f;
            }
            else
            {
                static_assert(false, "Unsupported type for ConstantTexture");
            }
        }
        else if constexpr (std::is_same_v<TextureType<T>, ImageTexture<T>>)
        {
            if constexpr (std::is_same_v<T, Float>)
            {
                return pool_2d1f;
            }
            else if constexpr (std::is_same_v<T, Spectrum>)
            {
                return pool_2d3f;
            }
            else
            {
                static_assert(false, "Unsupported type for ImageTexture");
            }
        }
        else if constexpr (std::is_same_v<TextureType<T>, CheckerTexture<T>>)
        {
            if constexpr (std::is_same_v<T, Float>)
            {
                return pool_C1f;
            }
            else if constexpr (std::is_same_v<T, Spectrum>)
            {
                return pool_C3f;
            }
            else
            {
                static_assert(false, "Unsupported type for CheckerTexture");
            }
        }
        else
        {
            static_assert(false, "Unsupported texture type");
        }
    }

    detail::Pool0d<Float> pool_0d1f;
    detail::Pool0d<Spectrum> pool_0d3f;

    detail::Pool2d<Float> pool_2d1f;
    detail::Pool2d<Spectrum> pool_2d3f;

    detail::PoolC<Float> pool_C1f;
    detail::PoolC<Spectrum> pool_C3f;
};

} // namespace bulbit
