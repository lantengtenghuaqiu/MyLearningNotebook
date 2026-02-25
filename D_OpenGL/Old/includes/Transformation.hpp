#ifndef MVP_TRANSFORM_MATRIX
#define MVP_TRANSFORM_MATRIX
#include <cmath>
#include <stdio.h>
#include "Data.hpp"
#define pi 3.1415926
#define rad pi / 180.0

float zero[16] = {
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f};

float identity[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f};

namespace Transform
{
    void Scale(float sx, float sy, float sz, float *_scale)
    {
        float scale[16] = {
            sx, 0.0f, 0.0f, 0.0f,
            0.0f, sy, 0.0f, 0.0f,
            0.0f, 0.0f, sz, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

        for (int i = 0; i < 16; i++)
        {
            _scale[i] = scale[i];
        }
    }
    void RotationX(float angle, float *_roation)
    {
        float _angle = angle * rad;
        float roation[16] = {
            1.0, 0.0f, 0.0f, 0.0f,
            0.0f, cos(_angle), -sin(_angle), 0.0f,
            0.0f, sin(_angle), cos(_angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

        for (int i = 0; i < 16; i++)
        {
            _roation[i] = roation[i];
        }
    }

    void RotationY(float angle, float *_roation)
    {
        float _angle = angle * rad;
        float roation[16] = {
            cos(_angle), 0.0f, sin(_angle), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sin(_angle), 0.0f, cos(_angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

        for (int i = 0; i < 16; i++)
        {
            _roation[i] = roation[i];
        }
    }
    void RotationZ(float angle, float *_roation)
    {
        float _angle = angle * rad;
        float roation[16] = {
            cos(_angle), -sin(_angle), 0.0f, 0.0f,
            sin(_angle), cos(_angle), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

        for (int i = 0; i < 16; i++)
        {
            _roation[i] = roation[i];
        }
    }
    void Rotation(float _x, float _y, float _z, float *_mat)
    {
        float x = _x * rad;
        float y = _y * rad;
        float z = _z * rad;

        float mat[16] = {
            cos(y) * cos(z), -cos(y) * sin(z), sin(y), 0.0f,
            sin(x) * sin(y) * cos(z) + cos(x) * sin(z), -sin(x) * sin(y) * sin(z) + cos(x) * cos(z), -sin(x) * cos(y), 0.0f,
            -cos(x) * sin(y) * cos(z) + sin(x) * sin(z), cos(x) * sin(y) * sin(z) + sin(x) * cos(z), cos(x) * cos(y), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

        for (int i = 0; i < 16; i++)
        {
            _mat[i] = mat[i];
        }
    }
    void Translate(float tx, float ty, float tz, float *_matrix)
    {
        float matrix[16] = {
            1.0f, 0.0f, 0.0f, tx,
            0.0f, 1.0f, 0.0f, ty,
            0.0f, 0.0f, 1.0f, tz,
            0.0f, 0.0f, 0.0f, 1.0f};

        for (int i = 0; i < 16; i++)
        {
            _matrix[i] = matrix[i];
        }
    }
}


namespace View
{
    // View Matrix
    // float Forward[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    // float Up[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    // float Left[4] = {
    //     Forward[0] * Up[1] - Forward[1] * Up[0],
    //     Forward[2] * Up[0] - Forward[0] * Up[2],
    //     Forward[1] * Up[2] - Forward[2] * Up[1],
    //     1.0f};

}


namespace Math
{
    Vec4 Cross(const Vec4 &a, const Vec4 &b)
    {
        Vec4 temp(0.0f, 0.0f, 0.0f, 0.0f);

        temp.x = a.y * b.z - a.z * b.y;
        temp.y = a.z * b.x - a.x * b.z;
        temp.z = a.x * b.y - a.y * b.x;
        temp.w = a.w;
        return temp;
    }
    float Dot(const Vec4 &a, const Vec4 &b)
    {
        return (a.x * b.x) + (a.y * b.y) + (a.z + b.z);
    }
}
#endif