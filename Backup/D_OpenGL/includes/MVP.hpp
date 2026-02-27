#ifndef MVP_TRANSFORM_MATRIX
#define MVP_TRANSFORM_MATRIX
#include <cmath>
#include <stdio.h>
#define pi M_PI
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

struct mat4
{
    float _mat4[16] = {0.0f};
    mat4() {};
    mat4(float *data)
    {
        for (int i = 0; i < 16; i++)
        {
            this->_mat4[i] = data[i];
        }
    }
};

mat4 operator*(mat4 a, mat4 b)
{
    mat4 temp;
    for (int k = 0; k < 4; k++)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp._mat4[k * 4 + i] += a._mat4[k * 4 + j] * b._mat4[j * 4 + i];
            }
        }
    }

    return temp;
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

class Vector4
{
public:
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float v[4];
    };

    Vector4()
    {
        for (int i = 0; i < 4; i++)
            this->v[i] = 0.0f;
    }

    Vector4(const float &x, const float &y, const float &z, const float &w)
    {
        this->v[0] = x;
        this->v[1] = y;
        this->v[2] = z;
        this->v[3] = w;
    }

    Vector4 &operator*(mat4 matrix)
    {
        Vector4 temp(0.0f, 0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp.v[i] += matrix._mat4[i * 4 + j] * this->v[j];
            }
        }
        return temp;
    }
};

namespace Math
{
    Vector4 Cross(const Vector4 &a, const Vector4 &b)
    {
        Vector4 temp(0.0f, 0.0f, 0.0f, 0.0f);

        temp.x = a.y * b.z - a.z * b.y;
        temp.y = a.z * b.x - a.x * b.z;
        temp.z = a.x * b.y - a.y * b.x;
        temp.w = a.w;
        return temp;
    }
    float Dot(const Vector4 &a, const Vector4 &b ){
        return (a.x*b.x)+(a.y*b.y)+(a.z+b.z);
    }
}
#endif