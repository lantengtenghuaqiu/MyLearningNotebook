#ifndef MVP_TRANSFORM_MATRIX
#define MVP_TRANSFORM_MATRIX
#include <cmath>
#include <stdio.h>
#include "Data.hpp"
#include "xyl_tools.hpp"

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
        // 优化改为对每个元素的修改:.
        _mat[0] = cos(y) * cos(z);
        _mat[1] = sin(x) * sin(y) * cos(z) + cos(x) * sin(z);
        _mat[2] = -cos(x) * sin(y) * cos(z) + sin(x) * sin(z);

        _mat[4] = -cos(y) * sin(z);
        _mat[5] = -sin(x) * sin(y) * sin(z) + cos(x) * cos(z);
        _mat[6] = cos(x) * sin(y) * sin(z) + sin(x) * cos(z);

        _mat[8] = sin(y);
        _mat[9] = -sin(x) * cos(y);
        _mat[10] = cos(x) * cos(y);

        // 优化前的矩阵形式:
        // float mat[16] = {
        //     cos(y) * cos(z), sin(x) * sin(y) * cos(z) + cos(x) * sin(z), -cos(x) * sin(y) * cos(z) + sin(x) * sin(z), 0.0f,
        //     -cos(y) * sin(z), -sin(x) * sin(y) * sin(z) + cos(x) * cos(z), cos(x) * sin(y) * sin(z) + sin(x) * cos(z), 0.0f,
        //     sin(y), -sin(x) * cos(y), cos(x) * cos(y), 0.0f,
        //     0.0f, 0.0f, 0.0f, 1.0f};

        // for (int i = 0; i < 16; i++)
        // {
        //     _mat[i] = mat[i];
        // }
    }

    void Rotation(Vec4 v4, float *_mat)
    {
        float x = v4.x * rad;
        float y = v4.y * rad;
        float z = v4.z * rad;

        _mat[0] = cos(y) * cos(z);
        _mat[1] = sin(x) * sin(y) * cos(z) + cos(x) * sin(z);
        _mat[2] = -cos(x) * sin(y) * cos(z) + sin(x) * sin(z);

        _mat[4] = -cos(y) * sin(z);
        _mat[5] = -sin(x) * sin(y) * sin(z) + cos(x) * cos(z);
        _mat[6] = cos(x) * sin(y) * sin(z) + sin(x) * cos(z);

        _mat[8] = sin(y);
        _mat[9] = -sin(x) * cos(y);
        _mat[10] = cos(x) * cos(y);
    }
    // void RotationX(float v, float *_matrix) { _matrix[12] = v4.x; }
    // void RotationY(float v, float *_matrix) { _matrix[12] = v4.x; }
    // void RotationZ(float v, float *_matrix) { _matrix[12] = v4.x; }

    void Translate(float tx, float ty, float tz, float *_matrix)
    {
        // float matrix[16] = {
        //     1.0f, 0.0f, 0.0f, 0.0f,
        //     0.0f, 1.0f, 0.0f, 0.0f,
        //     0.0f, 0.0f, 1.0f, 0.0f,
        //     tx, ty, tz, 1.0f};

        _matrix[12] += tx;
        _matrix[13] += ty;
        _matrix[14] += tz;

        // for (int i = 0; i < 16; i++)
        // {
        //     _matrix[i] = matrix[i];
        // }
    }
    void Translate(Vec4 v4, float *_matrix)
    {
        _matrix[12] += v4.x;
        _matrix[13] += v4.y;
        _matrix[14] += v4.z;
    }
    void TranslateX(float v, float *_matrix) { _matrix[12] = v; }
    void TranslateY(float v, float *_matrix) { _matrix[13] = v; }
    void TranslateZ(float v, float *_matrix) { _matrix[14] = v; }

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