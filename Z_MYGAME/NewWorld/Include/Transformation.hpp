#ifndef MVP_TRANSFORM_MATRIX
#define MVP_TRANSFORM_MATRIX

#include <cmath>

#include "./Data.hpp"

namespace TRANSFORM
{
    // Scale Transform
    void Scale(float sx, float sy, float sz, float *_matrix)
    {
        _matrix[0] = sx;
        _matrix[5] = sy;
        _matrix[10] = sz;
    }
    void Scale(Vec4 v4, float *_matrix)
    {
        _matrix[0] = v4.x;
        _matrix[5] = v4.y;
        _matrix[10] = v4.z;
    }

    // Rotation Matrix
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
    // void RotationX(Vec4 v4, float *_matrix) { _matrix[] = v4.x; }
    // void RotationY(Vec4 v4, float *_matrix) { _matrix[] = v4.y; }
    // void RotationZ(Vec4 v4, float *_matrix) { _matrix[] = v4.z; }

    void Translate(float tx, float ty, float tz, float *_matrix)
    {
        _matrix[12] = tx;
        _matrix[13] = ty;
        _matrix[14] = tz;
    }
    void Translate(Vec4 v4, float *_matrix)
    {
        _matrix[12] = v4.x;
        _matrix[13] = v4.y;
        _matrix[14] = v4.z;
    }
    void TranslateX(float x, float *_matrix) { _matrix[12] = x; }
    void TranslateY(float y, float *_matrix) { _matrix[13] = y; }
    void TranslateZ(float z, float *_matrix) { _matrix[14] = z; }

}

#endif