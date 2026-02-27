#ifndef DATA
#define DATA
#include<string.h>
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
};

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

    Vector4 operator*(Vector4 vec4)
    {

        Vector4 temp(0.0f, 0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp.v[i] += this->_mat4[i * 4 + j] * vec4.v[j];
            }
        }
        return temp;
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

class Vec4 : public Vector4
{
public:
    Vec4() : Vector4() {}

    Vec4(const float &x, const float &y, const float &z, const float &w): Vector4(x, y, z, w){}

    void Set(float x, float y, float z, float w) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    Vec4 operator*(mat4 matrix)
    {
        Vec4 temp(0.0f, 0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp.v[i] += matrix._mat4[i * 4 + j] * this->v[j];
            }
        }
        return temp;
    }

    Vec4& operator=(const Vector4 &vec4){
        if (this == &vec4) {
            return *this;
        }
        memcpy(this->v,vec4.v,sizeof(vec4.v));
        return *this;
    }
};

#endif