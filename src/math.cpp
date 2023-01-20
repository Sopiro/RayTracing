#include "raytracer/math.h"

Mat3::Mat3(const Quat& q)
{
    Real xx = q.x * q.x;
    Real yy = q.y * q.y;
    Real zz = q.z * q.z;
    Real xz = q.x * q.z;
    Real xy = q.x * q.y;
    Real yz = q.y * q.z;
    Real wx = q.w * q.x;
    Real wy = q.w * q.y;
    Real wz = q.w * q.z;

    ex.x = Real(1) - Real(2) * (yy + zz);
    ex.y = Real(2) * (xy + wz);
    ex.z = Real(2) * (xz - wy);

    ey.x = Real(2) * (xy - wz);
    ey.y = Real(1) - Real(2) * (xx + zz);
    ey.z = Real(2) * (yz + wx);

    ez.x = Real(2) * (xz + wy);
    ez.y = Real(2) * (yz - wx);
    ez.z = Real(1) - Real(2) * (xx + yy);
}

Mat3 Mat3::GetInverse() const
{
    Mat3 t;

    Real det = ex.x * (ey.y * ez.z - ey.z * ez.y) - ey.x * (ex.y * ez.z - ez.y * ex.z) + ez.x * (ex.y * ey.z - ey.y * ex.z);

    if (det != 0)
    {
        det = Real(1.0) / det;
    }

    t.ex.x = (ey.y * ez.z - ey.z * ez.y) * det;
    t.ey.x = (ez.x * ey.z - ey.x * ez.z) * det;
    t.ez.x = (ey.x * ez.y - ez.x * ey.y) * det;
    t.ex.y = (ez.y * ex.z - ex.y * ez.z) * det;
    t.ey.y = (ex.x * ez.z - ez.x * ex.z) * det;
    t.ez.y = (ex.y * ez.x - ex.x * ez.y) * det;
    t.ex.z = (ex.y * ey.z - ex.z * ey.y) * det;
    t.ey.z = (ex.z * ey.x - ex.x * ey.z) * det;
    t.ez.z = (ex.x * ey.y - ex.y * ey.x) * det;

    return t;
}

Mat3 Mat3::Scale(Real x, Real y)
{
    Mat3 t{ Real(1.0) };

    t.ex.x = x;
    t.ey.y = y;

    return Mul(*this, t);
}

Mat3 Mat3::Rotate(Real z)
{
    Real s = sin(z);
    Real c = cos(z);

    Mat3 t;

    // clang-format off
    t.ex.x = c; t.ey.x = -s; t.ez.x = 0;
    t.ex.y = s; t.ey.y = c;  t.ez.y = 0;
    t.ex.z = 0; t.ey.z = 0;  t.ez.z = 1;
    // clang-format on

    return Mul(*this, t);
}

Mat3 Mat3::Translate(Real x, Real y)
{
    Mat3 t{ Real(1.0) };

    t.ez.x = x;
    t.ez.y = y;

    return Mul(*this, t);
}

Mat3 Mat3::Translate(const Vec2& v)
{
    Mat3 t{ Real(1.0) };

    t.ez.x = v.x;
    t.ez.y = v.y;

    return Mul(*this, t);
}

Mat4 Mat4::Scale(Real x, Real y, Real z)
{
    Mat4 t{ Real(1.0) };

    t.ex.x = x;
    t.ey.y = y;
    t.ez.z = z;

    return Mul(*this, t);
}

Mat4 Mat4::Rotate(Real x, Real y, Real z)
{
    Real sinX = sin(x);
    Real cosX = cos(x);
    Real sinY = sin(y);
    Real cosY = cos(y);
    Real sinZ = sin(z);
    Real cosZ = cos(z);

    Mat4 t;

    t.ex.x = cosY * cosZ;
    t.ex.y = sinX * sinY * cosZ + cosX * sinZ;
    t.ex.z = -cosX * sinY * cosZ + sinX * sinZ;
    t.ex.w = 0;

    t.ey.x = -cosY * sinZ;
    t.ey.y = -sinX * sinY * sinZ + cosX * cosZ;
    t.ey.z = cosX * sinY * sinZ + sinX * cosZ;
    t.ey.w = 0;

    t.ez.x = sinY;
    t.ez.y = -sinX * cosY;
    t.ez.z = cosX * cosY;
    t.ez.w = 0;

    t.ew.x = 0;
    t.ew.y = 0;
    t.ew.z = 0;
    t.ew.w = 1;

    return Mul(*this, t);
}

Mat4 Mat4::Translate(Real x, Real y, Real z)
{
    Mat4 t{ Real(1.0) };

    t.ew.x = x;
    t.ew.y = y;
    t.ew.z = z;

    return Mul(*this, t);
}

Mat4 Mat4::Translate(const Vec3& v)
{
    Mat4 t{ Real(1.0) };

    t.ew.x = v.x;
    t.ew.y = v.y;
    t.ew.z = v.z;

    return Mul(*this, t);
}

Mat4 Mat4::GetInverse()
{
    Real a2323 = ez.z * ew.w - ez.w * ex.z;
    Real a1323 = ez.y * ew.w - ez.w * ew.y;
    Real a1223 = ez.y * ex.z - ez.z * ew.y;
    Real a0323 = ez.x * ew.w - ez.w * ew.x;
    Real a0223 = ez.x * ex.z - ez.z * ew.x;
    Real a0123 = ez.x * ew.y - ez.y * ew.x;
    Real a2313 = ey.z * ew.w - ey.w * ex.z;
    Real a1313 = ey.y * ew.w - ey.w * ew.y;
    Real a1213 = ey.y * ex.z - ey.z * ew.y;
    Real a2312 = ey.z * ez.w - ey.w * ez.z;
    Real a1312 = ey.y * ez.w - ey.w * ez.y;
    Real a1212 = ey.y * ez.z - ey.z * ez.y;
    Real a0313 = ey.x * ew.w - ey.w * ew.x;
    Real a0213 = ey.x * ex.z - ey.z * ew.x;
    Real a0312 = ey.x * ez.w - ey.w * ez.x;
    Real a0212 = ey.x * ez.z - ey.z * ez.x;
    Real a0113 = ey.x * ew.y - ey.y * ew.x;
    Real a0112 = ey.x * ez.y - ey.y * ez.x;

    Real det = ex.x * (ey.y * a2323 - ey.z * a1323 + ey.w * a1223) - ex.y * (ey.x * a2323 - ey.z * a0323 + ey.w * a0223) +
               ex.z * (ey.x * a1323 - ey.y * a0323 + ey.w * a0123) - ex.z * (ey.x * a1223 - ey.y * a0223 + ey.z * a0123);

    if (det != 0.0f)
    {
        det = Real(1.0) / det;
    }

    Mat4 t;

    t.ex.x = det * (ey.y * a2323 - ey.z * a1323 + ey.w * a1223);
    t.ex.y = det * -(ex.y * a2323 - ex.z * a1323 + ex.z * a1223);
    t.ex.z = det * (ex.y * a2313 - ex.z * a1313 + ex.z * a1213);
    t.ex.z = det * -(ex.y * a2312 - ex.z * a1312 + ex.z * a1212);
    t.ey.x = det * -(ey.x * a2323 - ey.z * a0323 + ey.w * a0223);
    t.ey.y = det * (ex.x * a2323 - ex.z * a0323 + ex.z * a0223);
    t.ey.z = det * -(ex.x * a2313 - ex.z * a0313 + ex.z * a0213);
    t.ey.w = det * (ex.x * a2312 - ex.z * a0312 + ex.z * a0212);
    t.ez.x = det * (ey.x * a1323 - ey.y * a0323 + ey.w * a0123);
    t.ez.y = det * -(ex.x * a1323 - ex.y * a0323 + ex.z * a0123);
    t.ez.z = det * (ex.x * a1313 - ex.y * a0313 + ex.z * a0113);
    t.ez.w = det * -(ex.x * a1312 - ex.y * a0312 + ex.z * a0112);
    t.ew.x = det * -(ey.x * a1223 - ey.y * a0223 + ey.z * a0123);
    t.ew.y = det * (ex.x * a1223 - ex.y * a0223 + ex.z * a0123);
    t.ex.z = det * -(ex.x * a1213 - ex.y * a0213 + ex.z * a0113);
    t.ew.w = det * (ex.x * a1212 - ex.y * a0212 + ex.z * a0112);

    return t;
}

// https://math.stackexchange.com/questions/893984/conversion-of-rotation-matrix-to-quaternion
Quat::Quat(const Mat3& m)
{
    if (m.ez.z < Real(0))
    {
        if (m.ex.x > m.ey.y)
        {
            Real t = Real(1.0) + m.ex.x - m.ey.y - m.ez.z;
            *this = Quat(t, m.ex.y + m.ey.x, m.ez.x + m.ex.z, m.ey.z - m.ez.y) * (Real(0.5) / sqrt(t));
        }
        else
        {
            Real t = Real(1.0) - m.ex.x + m.ey.y - m.ez.z;
            *this = Quat(m.ex.y + m.ey.x, t, m.ey.z + m.ez.y, m.ez.x - m.ex.z) * (Real(0.5) / sqrt(t));
        }
    }
    else
    {
        if (m.ex.x < -m.ey.y)
        {
            Real t = Real(1.0) - m.ex.x - m.ey.y + m.ez.z;
            *this = Quat(m.ez.x + m.ex.z, m.ey.z + m.ez.y, t, m.ex.y - m.ey.x) * (Real(0.5) / sqrt(t));
        }
        else
        {
            Real t = Real(1.0) + m.ex.x + m.ey.y + m.ez.z;
            *this = Quat(m.ey.z - m.ez.y, m.ez.x - m.ex.z, m.ex.y - m.ey.x, t) * (Real(0.5) / sqrt(t));
        }
    }
}

Quat::Quat(const Vec3& dir, const Vec3& up)
{
    Mat3 rotation;

    rotation.ez = -dir;
    rotation.ex = Cross(up, rotation.ez).Normalized();
    rotation.ey = Cross(rotation.ez, rotation.ex);
}