#include "Vector.h"

const Vec2 Vec2_Right = { +1.0f, +0.0f };
const Vec2 Vec2_Left  = { -1.0f, +0.0f };
const Vec2 Vec2_Up    = { +0.0f, +1.0f };
const Vec2 Vec2_Down  = { +0.0f, -1.0f };
const Vec2 Vec2_Zero  = { +0.0f, +0.0f };
const Vec2 Vec2_One   = { +1.0f, +1.0f };

const Vec3 Vec3_Right = { +1.0f, +0.0f, +0.0f };
const Vec3 Vec3_Left  = { -1.0f, +0.0f, +0.0f };
const Vec3 Vec3_Up    = { +0.0f, +1.0f, +0.0f };
const Vec3 Vec3_Down  = { +0.0f, -1.0f, +0.0f };
const Vec3 Vec3_Front = { +0.0f, +0.0f, +1.0f };
const Vec3 Vec3_Back  = { +0.0f, +0.0f, -1.0f };
const Vec3 Vec3_Zero  = { +0.0f, +0.0f, +0.0f };
const Vec3 Vec3_One   = { +1.0f, +1.0f, +1.0f };

const Vec4 Vec4_RightH = { +1.0f, +0.0f, +0.0f, +1.0f };
const Vec4 Vec4_LeftH  = { -1.0f, +0.0f, +0.0f, +1.0f };
const Vec4 Vec4_UpH    = { +0.0f, +1.0f, +0.0f, +1.0f };
const Vec4 Vec4_DownH  = { +0.0f, -1.0f, +0.0f, +1.0f };
const Vec4 Vec4_FrontH = { +0.0f, +0.0f, +1.0f, +1.0f };
const Vec4 Vec4_BackH  = { +0.0f, +0.0f, -1.0f, +1.0f };
const Vec4 Vec4_ZeroH  = { +0.0f, +0.0f, +0.0f, +1.0f };
const Vec4 Vec4_Zero   = { +0.0f, +0.0f, +0.0f, +0.0f };
const Vec4 Vec4_One    = { +1.0f, +1.0f, +1.0f, +1.0f };

//-------------------------------------------------------------------------------------------------
// Fonctions à coder

// TODO
Vec2 Vec2_Add(Vec2 v1, Vec2 v2)
{
    v1.x += v2.x;
    v1.y += v2.y;

    return v1;
}

// TODO
Vec2 Vec2_Sub(Vec2 v1, Vec2 v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;

    return v1;
}

// TODO
float Vec2_SignedArea(const Vec2 a, const Vec2 b, const Vec2 c)
{
    float res = 0.f;
    res = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);

    return res;
}

// TODO
bool Vec2_Barycentric(Vec2 *vertices, Vec2 p, float *bary)
{
    memset(bary, 0, 3 * sizeof(float));

    Vec2 bmp = Vec2_Sub(vertices[1], p);
    Vec2 cmp = Vec2_Sub(vertices[2], p);
    Vec2 bma = Vec2_Sub(vertices[1], vertices[0]);
    Vec2 cma = Vec2_Sub(vertices[2], vertices[0]);
    Vec2 amp = Vec2_Sub(vertices[0], p);

    float det0 = bmp.x * cmp.y - cmp.x * bmp.y;
    float deT = bma.x * cma.y - cma.x * bma.y;
    float det1 = cmp.x * amp.y - amp.x * cmp.y;
    float det2 = amp.x * bmp.y - bmp.x * amp.y;

    bary[0] = det0 / deT;
    bary[1] = det1 / deT;
    bary[2] = det2 / deT;

    if (bary[0] > 0 && bary[1] > 0 && bary[2] > 0)
        return true;

    return false;
}

// TODO
Vec3 Vec3_Add(Vec3 v1, Vec3 v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;

    return v1;
}

// TODO
Vec3 Vec3_Sub(Vec3 v1, Vec3 v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;

    return v1;
}

// TODO
Vec3 Vec3_Scale(Vec3 v, float s)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;

    return v;
}

// TODO
float Vec3_Dot(Vec3 v1, Vec3 v2)
{
    float res = 0.f;
    res = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

    return res;
}

// TODO
Vec3 Vec3_Cross(Vec3 v1, Vec3 v2)
{
    Vec3 res = { 0 };
    res.x = v1.y * v2.z - v1.z * v2.y;
    res.y = v1.z * v2.x - v1.x * v2.z;
    res.z = v1.x * v2.y - v1.y * v2.x;

    return res;
}

// TODO
float Vec3_Length(Vec3 v)
{
    float length = 0.f;
    length = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));

    return length;
}

// TODO
Vec3 Vec3_Normalize(Vec3 v)
{
    float length = Vec3_Length(v);
    if (length == 1)
        return v;
    v.x /= length;
    v.y /= length;
    v.z /= length;

    return v;
}

// TODO
Vec3 Vec3_From4(Vec4 v)
{
    Vec3 res = { 0 };
    if (v.w == 0) {
        res.x = v.x;
        res.y = v.y;
        res.z = v.z;
        return res;
    }
    res.x = v.x / v.w;
    res.y = v.y / v.w;
    res.z = v.z / v.w;
    return res;
}

//-------------------------------------------------------------------------------------------------
// Fonctions du basecode

Vec3 Vec3_Mul(Vec3 v1, Vec3 v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    return v1;
}

Vec3 Vec3_Max(Vec3 v1, Vec3 v2)
{
    v1.x = fmaxf(v1.x, v2.x);
    v1.y = fmaxf(v1.y, v2.y);
    v1.z = fmaxf(v1.z, v2.z);
    return v1;
}

Vec3 Vec3_Min(Vec3 v1, Vec3 v2)
{
    v1.x = fminf(v1.x, v2.x);
    v1.y = fminf(v1.y, v2.y);
    v1.z = fminf(v1.z, v2.z);
    return v1;
}
