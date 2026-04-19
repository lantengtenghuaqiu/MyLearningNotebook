#ifndef XMATH
#define XMATH
#include "./Data.hpp"
namespace Math
{
    Vec4 Cross(const Vec4 &a, const Vec4 &b)
    {
        Vec4 temp(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x,
            a.w);

        return temp;
    }

    Vec4 Cross(const Vec4 &a, float x, float y, float z)
    {
        Vec4 temp(
            a.y * z - a.z * y,
            a.z * x - a.x * z,
            a.x * y - a.y * x,
            a.w);

        return temp;
    }

    float Dot(const Vec4 &a, const Vec4 &b)
    {
        return (a.x * b.x) + (a.y * b.y) + (a.z + b.z);
    }
}
#endif