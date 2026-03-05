#ifndef DATA
#define DATA
#include <string.h>
// #include <unordered_map>
#include <vector>
#define pi 3.1415926
#define rad 0.01745329
struct Vector4
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float v4[4];
    };
    float v3[3];
    Vector4()
    {
        for (int i = 0; i < 4; i++)
        {
            this->v4[i] = 0.0f;
            if (i < 3)
            {
                this->v3[i] = 0.0f;
            }
        }
    }
    Vector4(const float &x, const float &y, const float &z, const float &w)
    {
        this->v4[0] = x;
        this->v4[1] = y;
        this->v4[2] = z;
        this->v4[3] = w;

        this->v3[0] = x;
        this->v3[1] = y;
        this->v3[2] = z;
    }
};

struct mat4
{
    float _mat4[16] = {0.0f};
    mat4()
    {
        this->_mat4[0] = 1.0f;
        this->_mat4[5] = 1.0f;
        this->_mat4[10] = 1.0f;
        this->_mat4[15] = 1.0f;
    };
    mat4(float *data)
    {
        for (int i = 0; i < 16; i++)
        {
            this->_mat4[i] = data[i];
        }
    }

    Vector4 operator*(Vector4 vec4)
    {

        Vector4 temp(0.0f, 0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp.v4[i] += this->_mat4[j * 4 + i] * vec4.v4[j];
            }
        }
        return temp;
    }

    // mat4 &operator=(const mat4 &other){
    //     for(int i = 0 ; i<16 ; i++){
    //         this->_mat4[i] = other._mat4[i];
    //     }

    //     return *this;
    // }

    mat4(const mat4 &) = default;

    mat4(mat4 &&) = default;

    mat4 &operator=(const mat4 &) = default;

    mat4 &operator=(mat4 &&) = default;
};

mat4 operator*(mat4 a, mat4 b)
{
    mat4 res;
    for (int col = 0; col < 4; col++)
    { // 列（OpenGL 优先）
        for (int row = 0; row < 4; row++)
        { // 行
            float sum = 0.0f;
            for (int k = 0; k < 4; k++)
            {
                // 列主序标准计算公式
                sum += a._mat4[k * 4 + row] * b._mat4[col * 4 + k];
            }
            res._mat4[col * 4 + row] = sum;
        }
    }
    return res;
}

class Vec4 : public Vector4
{
public:
    Vec4() : Vector4() {}

    Vec4(const float &x, const float &y, const float &z, const float &w) : Vector4(x, y, z, w) {}

    void Set(float x, float y, float z, float w)
    {
        this->v4[0] = x;
        this->v4[1] = y;
        this->v4[2] = z;
        this->v4[3] = w;

        this->v3[0] = x;
        this->v3[1] = y;
        this->v3[2] = z;
    }
    void Set(Vec4 v4)
    {
        this->v4[0] = v4.x;
        this->v4[1] = v4.y;
        this->v4[2] = v4.z;
        this->v4[3] = v4.w;

        this->v3[0] = v4.x;
        this->v3[1] = v4.y;
        this->v3[2] = v4.z;
    }
    Vec4 operator*(float scale)
    {
        Vec4 temp;
        temp.x = this->x * scale;
        temp.y = this->y * scale;
        temp.z = this->z * scale;
        temp.w = this->w * scale;
        return temp;
    }

    Vec4 operator-(Vec4 other)
    {
        Vec4 temp;
        temp.x = this->x - other.x;
        temp.y = this->y - other.y;
        temp.z = this->z - other.z;
        temp.w = this->w - other.w;
        return temp;
    }

    Vec4 &operator=(const Vector4 &vec4)
    {
        if (this == &vec4)
        {
            return *this;
        }
        memcpy(this->v4, vec4.v4, sizeof(vec4.v4));
        return *this;
    }
    void SetV(float x, float y, float z, float w)
    {
        this->v4[0] = x;
        this->v4[1] = y;
        this->v4[2] = z;
        this->v4[3] = w;

        this->v3[0] = x;
        this->v3[1] = y;
        this->v3[2] = z;
    }
    void SetV(Vector4 v)
    {
        this->v4[0] = v.v4[0];
        this->v4[1] = v.v4[1];
        this->v4[2] = v.v4[2];
        this->v4[3] = v.v4[3];

        this->v3[0] = v.v4[0];
        this->v3[1] = v.v4[1];
        this->v3[2] = v.v4[2];
    }
};
Vector4 Normalize(Vector4 a)
{
    float root = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    a.v4[0] = a.v4[0] / root;
    a.v4[1] = a.v4[1] / root;
    a.v4[2] = a.v4[2] / root;

    a.v3[0] = a.v3[0] / root;
    a.v3[1] = a.v3[1] / root;
    a.v3[2] = a.v3[2] / root;

    return a;
}

Vec4 Normalize(float x, float y, float z, float w)
{
    float root = sqrt(x * x + y * y + z * z);
    Vec4 temp(x / root, y / root, z / root, w);
    return temp;
}

#endif