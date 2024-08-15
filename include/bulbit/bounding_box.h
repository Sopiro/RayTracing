#pragma once

#include "ray.h"

namespace bulbit
{

template <typename T>
struct BoundingBox2;
template <typename T>
struct BoundingBox3;

using AABB2 = BoundingBox2<Float>;
using AABB2i = BoundingBox2<int32>;

using AABB3 = BoundingBox3<Float>;
using AABB3i = BoundingBox3<int32>;

using AABB = AABB3;

template <typename T>
struct BoundingBox2
{
    BoundingBox2();
    BoundingBox2(const Vector2<T>& min, const Vector2<T>& max);

    Vector2<T> operator[](int32 i) const;
    Vector2<T>& operator[](int32 i);

    Vector2<T> GetCenter() const;
    Vector2<T> GetExtents() const;

    T GetSurfaceArea() const;
    T GetPerimeter() const;

    bool Contains(const BoundingBox2& other) const;
    bool TestPoint(const Vector2<T>& point) const;
    bool TestOverlap(const BoundingBox2& other) const;

    bool TestRay(const Ray& ray, Float t_min, Float t_max) const;
    bool TestRay(Vec2 o, Float t_min, Float t_max, Vec2 inv_dir, const int is_neg_dir[2]) const;
    Float Intersect(const Ray& ray, Float t_min, Float t_max) const;

    void ComputeBoundingCircle(Point2* center, T* radius) const;

    std::string ToString() const;

    Vector2<T> min, max;

    static BoundingBox2 Union(const BoundingBox2& b1, const BoundingBox2& b2);
    static BoundingBox2 Union(const BoundingBox2& aabb, const Vector2<T>& p);
};

template <typename T>
inline BoundingBox2<T> BoundingBox2<T>::Union(const BoundingBox2& aabb1, const BoundingBox2& aabb2)
{
    Vector2<T> min = Min(aabb1.min, aabb2.min);
    Vector2<T> max = Max(aabb1.max, aabb2.max);

    return BoundingBox2{ min, max };
}

template <typename T>
inline BoundingBox2<T> BoundingBox2<T>::Union(const BoundingBox2& aabb, const Vector2<T>& point)
{
    Vector2<T> min = Min(aabb.min, point);
    Vector2<T> max = Max(aabb.max, point);

    return BoundingBox2{ min, max };
}

template <typename T>
inline BoundingBox2<T>::BoundingBox2()
    : min{ max_value }
    , max{ -max_value }
{
}

template <typename T>
inline BoundingBox2<T>::BoundingBox2(const Vector2<T>& min, const Vector2<T>& max)
    : min{ min }
    , max{ max }
{
}

template <typename T>
inline Vector2<T>& BoundingBox2<T>::operator[](int32 i)
{
    BulbitAssert(i == 0 || i == 1);
    return (i == 0) ? min : max;
}

template <typename T>
inline Vector2<T> BoundingBox2<T>::operator[](int32 i) const
{
    BulbitAssert(i == 0 || i == 1);
    return (i == 0) ? min : max;
}

template <typename T>
inline Vector2<T> BoundingBox2<T>::GetCenter() const
{
    return (min + max) * 0.5f;
}

template <typename T>
inline Vector2<T> BoundingBox2<T>::GetExtents() const
{
    return (max - min);
}

template <typename T>
inline T BoundingBox2<T>::GetSurfaceArea() const
{
    return (max.x - min.x) * (max.y - min.y);
}

template <typename T>
inline T BoundingBox2<T>::GetPerimeter() const
{
    Vector2<T> w = max - min;
    return 2 * (w.x + w.y);
}

template <typename T>
inline bool BoundingBox2<T>::Contains(const BoundingBox2& other) const
{
    return min.x <= other.min.x && min.y <= other.min.y && max.x >= other.max.x && max.y >= other.max.y;
}

template <typename T>
inline bool BoundingBox2<T>::TestPoint(const Vector2<T>& point) const
{
    if (min.x > point.x || max.x < point.x) return false;
    if (min.y > point.y || max.y < point.y) return false;

    return true;
}

template <typename T>
inline bool BoundingBox2<T>::TestOverlap(const BoundingBox2& other) const
{
    if (min.x > other.max.x || max.x < other.min.x) return false;
    if (min.y > other.max.y || max.y < other.min.y) return false;

    return true;
}

// https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/anoptimizedaabbhitmethod
template <typename T>
inline bool BoundingBox2<T>::TestRay(const Ray& ray, Float t_min, Float t_max) const
{
    for (int32 axis = 0; axis < 2; ++axis)
    {
        Float invD = 1 / ray.d[axis];
        Float origin = ray.o[axis];

        Float t0 = (min[axis] - origin) * invD;
        Float t1 = (max[axis] - origin) * invD;

        if (invD < 0)
        {
            std::swap(t0, t1);
        }

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        if (t_max <= t_min)
        {
            return false;
        }
    }

    return true;
}

// https://www.pbr-book.org/4ed/Shapes/Basic_Shape_Interface#Bounds3::IntersectP
template <typename T>
inline bool BoundingBox2<T>::TestRay(Vec2 o, Float t_min, Float t_max, Vec2 inv_dir, const int is_neg_dir[2]) const
{
    const BoundingBox2<Float>& aabb = *this;

    // Slab test for x component
    Float t_min_x = (aabb[is_neg_dir[0]].x - o.x) * inv_dir.x;
    Float t_max_x = (aabb[1 - is_neg_dir[0]].x - o.x) * inv_dir.x;

    if (t_min > t_max_x || t_max < t_min_x)
    {
        return false;
    }

    if (t_min_x > t_min) t_min = t_min_x;
    if (t_max_x < t_max) t_max = t_max_x;

    // Slab test for y component
    Float t_min_y = (aabb[is_neg_dir[1]].y - o.y) * inv_dir.y;
    Float t_max_y = (aabb[1 - is_neg_dir[1]].y - o.y) * inv_dir.y;

    if (t_min > t_max_y || t_max < t_min_y)
    {
        return false;
    }

    // if (t_min_y > t_min) t_min = t_min_y;
    // if (t_max_y < t_max) t_max = t_max_y;

    return true;
}

template <typename T>
inline Float BoundingBox2<T>::Intersect(const Ray& ray, Float t_min, Float t_max) const
{
    for (int32 axis = 0; axis < 2; ++axis)
    {
        Float invD = 1 / ray.d[axis];
        Float origin = ray.o[axis];

        Float t0 = (min[axis] - origin) * invD;
        Float t1 = (max[axis] - origin) * invD;

        if (invD < 0)
        {
            std::swap(t0, t1);
        }

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        if (t_max <= t_min)
        {
            return infinity;
        }
    }

    return t_min;
}

template <typename T>
inline void BoundingBox2<T>::ComputeBoundingCircle(Point2* center, T* radius) const
{
    *center = GetCenter();
    *radius = TestPoint(*center) ? Dist(*center, max) : 0;
}

template <typename T>
inline std::string BoundingBox2<T>::ToString() const
{
    return std::format("min:\t{}\nmax:\t{}", min.ToString(), max.ToString());
}

template <typename T>
struct BoundingBox3
{
    BoundingBox3();
    BoundingBox3(const Vector3<T>& min, const Vector3<T>& max);

    Vector3<T> operator[](int32 i) const;
    Vector3<T>& operator[](int32 i);

    Vector3<T> GetCenter() const;
    Vector3<T> GetExtents() const;

    T GetVolume() const;
    T GetSurfaceArea() const;

    bool Contains(const BoundingBox3& other) const;
    bool TestPoint(const Vector3<T>& point) const;
    bool TestOverlap(const BoundingBox3& other) const;
    bool TestRay(const Ray& ray, Float t_min, Float t_max) const;
    bool TestRay(Vec3 o, Float t_min, Float t_max, Vec3 inv_dir, const int is_neg_dir[3]) const;
    Float Intersect(const Ray& ray, Float t_min, Float t_max) const;

    void ComputeBoundingSphere(Point3* center, T* radius) const;

    std::string ToString() const;

    Vector3<T> min, max;

    static BoundingBox3 Union(const BoundingBox3& b1, const BoundingBox3& b2);
    static BoundingBox3 Union(const BoundingBox3& aabb, const Vector3<T>& p);
};

template <typename T>
inline BoundingBox3<T> BoundingBox3<T>::Union(const BoundingBox3& aabb1, const BoundingBox3& aabb2)
{
    Vector3<T> min = Min(aabb1.min, aabb2.min);
    Vector3<T> max = Max(aabb1.max, aabb2.max);

    return BoundingBox3{ min, max };
}

template <typename T>
inline BoundingBox3<T> BoundingBox3<T>::Union(const BoundingBox3& aabb, const Vector3<T>& point)
{
    Vector3<T> min = Min(aabb.min, point);
    Vector3<T> max = Max(aabb.max, point);

    return BoundingBox3{ min, max };
}

template <typename T>
inline BoundingBox3<T>::BoundingBox3()
    : min{ max_value }
    , max{ -max_value }
{
}

template <typename T>
inline BoundingBox3<T>::BoundingBox3(const Vector3<T>& min, const Vector3<T>& max)
    : min{ min }
    , max{ max }
{
}

template <typename T>
inline Vector3<T>& BoundingBox3<T>::operator[](int32 i)
{
    BulbitAssert(i == 0 || i == 1);
    return (i == 0) ? min : max;
}

template <typename T>
inline Vector3<T> BoundingBox3<T>::operator[](int32 i) const
{
    BulbitAssert(i == 0 || i == 1);
    return (i == 0) ? min : max;
}

template <typename T>
inline Vector3<T> BoundingBox3<T>::GetCenter() const
{
    return (min + max) * 0.5f;
}

template <typename T>
inline Vector3<T> BoundingBox3<T>::GetExtents() const
{
    return (max - min);
}

template <typename T>
inline T BoundingBox3<T>::GetVolume() const
{
    return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
}

template <typename T>
inline T BoundingBox3<T>::GetSurfaceArea() const
{
    Vector3<T> w = max - min;
    return 2 * ((w.x * w.y) + (w.y * w.z) + (w.z * w.x));
}

template <typename T>
inline bool BoundingBox3<T>::Contains(const BoundingBox3& other) const
{
    return min.x <= other.min.x && min.y <= other.min.y && min.z <= other.min.z && max.x >= other.max.x && max.y >= other.max.y &&
           max.z >= other.max.z;
}

template <typename T>
inline bool BoundingBox3<T>::TestPoint(const Vector3<T>& point) const
{
    if (min.x > point.x || max.x < point.x) return false;
    if (min.y > point.y || max.y < point.y) return false;
    if (min.z > point.z || max.z < point.z) return false;

    return true;
}

template <typename T>
inline bool BoundingBox3<T>::TestOverlap(const BoundingBox3& other) const
{
    if (min.x > other.max.x || max.x < other.min.x) return false;
    if (min.y > other.max.y || max.y < other.min.y) return false;
    if (min.z > other.max.z || max.z < other.min.z) return false;

    return true;
}

// https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/anoptimizedaabbhitmethod
template <typename T>
inline bool BoundingBox3<T>::TestRay(const Ray& ray, Float t_min, Float t_max) const
{
    for (int32 axis = 0; axis < 3; ++axis)
    {
        Float invD = 1 / ray.d[axis];
        Float origin = ray.o[axis];

        Float t0 = (min[axis] - origin) * invD;
        Float t1 = (max[axis] - origin) * invD;

        if (invD < 0)
        {
            std::swap(t0, t1);
        }

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        if (t_max <= t_min)
        {
            return false;
        }
    }

    return true;
}

// https://www.pbr-book.org/4ed/Shapes/Basic_Shape_Interface#Bounds3::IntersectP
template <typename T>
inline bool BoundingBox3<T>::TestRay(Vec3 o, Float t_min, Float t_max, Vec3 inv_dir, const int is_neg_dir[3]) const
{
    const BoundingBox3<Float>& aabb = *this;

    // Slab test for x component
    Float t_min_x = (aabb[is_neg_dir[0]].x - o.x) * inv_dir.x;
    Float t_max_x = (aabb[1 - is_neg_dir[0]].x - o.x) * inv_dir.x;

    if (t_min > t_max_x || t_max < t_min_x)
    {
        return false;
    }

    if (t_min_x > t_min) t_min = t_min_x;
    if (t_max_x < t_max) t_max = t_max_x;

    // Slab test for y component
    Float t_min_y = (aabb[is_neg_dir[1]].y - o.y) * inv_dir.y;
    Float t_max_y = (aabb[1 - is_neg_dir[1]].y - o.y) * inv_dir.y;

    if (t_min > t_max_y || t_max < t_min_y)
    {
        return false;
    }

    if (t_min_y > t_min) t_min = t_min_y;
    if (t_max_y < t_max) t_max = t_max_y;

    Float t_min_z = (aabb[is_neg_dir[2]].z - o.z) * inv_dir.z;
    Float t_max_z = (aabb[1 - is_neg_dir[2]].z - o.z) * inv_dir.z;

    if (t_min > t_max_z || t_max < t_min_z)
    {
        return false;
    }

    // if (t_max_z > t_min) t_min = t_max_z;
    // if (t_max_z < t_max) t_max = t_max_z;

    return true;
}

template <typename T>
inline Float BoundingBox3<T>::Intersect(const Ray& ray, Float t_min, Float t_max) const
{
    for (int32 axis = 0; axis < 3; ++axis)
    {
        Float invD = 1 / ray.d[axis];
        Float origin = ray.o[axis];

        Float t0 = (min[axis] - origin) * invD;
        Float t1 = (max[axis] - origin) * invD;

        if (invD < 0)
        {
            std::swap(t0, t1);
        }

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        if (t_max <= t_min)
        {
            return infinity;
        }
    }

    return t_min;
}

template <typename T>
inline void BoundingBox3<T>::ComputeBoundingSphere(Point3* center, T* radius) const
{
    *center = GetCenter();
    *radius = TestPoint(*center) ? Dist(*center, max) : 0;
}

template <typename T>
inline std::string BoundingBox3<T>::ToString() const
{
    return std::format("min:\t{}\nmax:\t{}", min.ToString(), max.ToString());
}

// Iterators

class IteratorB2i : public std::forward_iterator_tag
{
public:
    IteratorB2i(const AABB2i& b, const Point2i& p)
        : p{ p }
        , bounds{ &b }
    {
    }

    IteratorB2i operator++()
    {
        advance();
        return *this;
    }

    IteratorB2i operator++(int32)
    {
        IteratorB2i old = *this;
        advance();
        return old;
    }

    bool operator==(const IteratorB2i& bi) const
    {
        return p == bi.p && bounds == bi.bounds;
    }

    bool operator!=(const IteratorB2i& bi) const
    {
        return p != bi.p || bounds != bi.bounds;
    }

    Point2i operator*() const
    {
        return p;
    }

private:
    void advance()
    {
        ++p.x;
        if (p.x == bounds->max.x)
        {
            p.x = bounds->min.x;
            ++p.y;
        }
    }

    Point2i p;
    const AABB2i* bounds;
};

class IteratorB3i : public std::forward_iterator_tag
{
public:
    IteratorB3i(const AABB3i& b, const Point3i& p)
        : p{ p }
        , bounds{ &b }
    {
    }

    IteratorB3i operator++()
    {
        advance();
        return *this;
    }

    IteratorB3i operator++(int32)
    {
        IteratorB3i old = *this;
        advance();
        return old;
    }

    bool operator==(const IteratorB3i& bi) const
    {
        return p == bi.p && bounds == bi.bounds;
    }

    bool operator!=(const IteratorB3i& bi) const
    {
        return p != bi.p || bounds != bi.bounds;
    }

    Point3i operator*() const
    {
        return p;
    }

private:
    void advance()
    {
        ++p.x;
        if (p.x == bounds->max.x)
        {
            p.x = bounds->min.x;
            ++p.y;

            if (p.y == bounds->max.y)
            {
                p.y = bounds->min.y;
                ++p.z;
            }
        }
    }

    Point3i p;
    const AABB3i* bounds;
};

inline IteratorB2i begin(const AABB2i& b)
{
    return IteratorB2i(b, b.min);
}

inline IteratorB2i end(const AABB2i& b)
{
    Point2i end(b.min.x, b.max.y);
    if (b.min.x >= b.max.x || b.min.y >= b.max.y)
    {
        end = b.min;
    }

    return IteratorB2i(b, end);
}

inline IteratorB3i begin(const AABB3i& b)
{
    return IteratorB3i(b, b.min);
}

inline IteratorB3i end(const AABB3i& b)
{
    Point3i end(b.min.x, b.min.y, b.max.z);
    if (b.min.x >= b.max.x || b.min.y >= b.max.y || b.min.z >= b.max.z)
    {
        end = b.min;
    }

    return IteratorB3i(b, end);
}

} // namespace bulbit