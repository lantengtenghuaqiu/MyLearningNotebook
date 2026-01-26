#include <stdio.h>

template <typename T>
class coordinate4
{
protected:
    T coordinate[4] = {};

public:
coordinate4(){}
    T &operator[](int i)
    {
        if (i >= 0 && i <= 3)
            return this->coordinate[i];
        else
            return this->coordinate[0];
    }
    coordinate4(const coordinate4 &other)
    {
        this->coordinate[0] = other.coordinate[0];
        this->coordinate[1] = other.coordinate[1];
        this->coordinate[2] = other.coordinate[2];
        this->coordinate[3] = other.coordinate[3];
    }
    ~coordinate4() {}

    coordinate4 &operator=(const coordinate4 &other)
    {
        this->coordinate[0] = other.coordinate[0];
        this->coordinate[1] = other.coordinate[1];
        this->coordinate[2] = other.coordinate[2];
        this->coordinate[3] = other.coordinate[3];
        return *this;
    }
};


class float4 : public coordinate4<float>
{
public:
    float4() {}
    float4(float x, float y, float z, float w)
    {
        this->coordinate[0] = x;
        this->coordinate[1] = y;
        this->coordinate[2] = z;
        this->coordinate[3] = w;
    };
    ~float4() {}
    float4(const float4 &other)
    {
        this->coordinate[0] = other.coordinate[0];
        this->coordinate[1] = other.coordinate[1];
        this->coordinate[2] = other.coordinate[2];
        this->coordinate[3] = other.coordinate[3];
    }
    float4 operator*(const float4 &other) const
    {
        float4 temp;
        temp.coordinate[0] = this->coordinate[0] * other.coordinate[0];
        temp.coordinate[1] = this->coordinate[1] * other.coordinate[1];
        temp.coordinate[2] = this->coordinate[2] * other.coordinate[2];
        temp.coordinate[3] = this->coordinate[3] * other.coordinate[3];
        return temp;
    }
    float4 operator/(const float4 &other) const
    {
        float4 temp;
        temp.coordinate[0] = this->coordinate[0] / other.coordinate[0];
        temp.coordinate[1] = this->coordinate[1] / other.coordinate[1];
        temp.coordinate[2] = this->coordinate[2] / other.coordinate[2];
        temp.coordinate[3] = this->coordinate[3] / other.coordinate[3];
        return temp;
    }
    float4 operator+(const float4 &other) const
    {
        float4 temp;
        temp.coordinate[0] = this->coordinate[0] + other.coordinate[0];
        temp.coordinate[1] = this->coordinate[1] + other.coordinate[1];
        temp.coordinate[2] = this->coordinate[2] + other.coordinate[2];
        temp.coordinate[3] = this->coordinate[3] + other.coordinate[3];
        return temp;
    }

    float4 operator-(const float4 &other) const
    {
        float4 temp;
        temp.coordinate[0] = this->coordinate[0] - other.coordinate[0];
        temp.coordinate[1] = this->coordinate[1] - other.coordinate[1];
        temp.coordinate[2] = this->coordinate[2] - other.coordinate[2];
        temp.coordinate[3] = this->coordinate[3] - other.coordinate[3];
        return temp;
    }
    float4 &operator=(const float4 &other)
    {
        if (this == &other)
            return *this;
        this->coordinate[0] = other.coordinate[0];
        this->coordinate[1] = other.coordinate[1];
        this->coordinate[2] = other.coordinate[2];
        this->coordinate[3] = other.coordinate[3];
        return *this;
    }
};

template <typename T>
class mat4x4
{
public:
    coordinate4<T> coordinate1;
    coordinate4<T> coordinate2;
    coordinate4<T> coordinate3;
    coordinate4<T> coordinate4;

    mat4x4() {}
    mat4x4(coordinate4<T> &c1,
           coordinate4<T> &c2,
           coordinate4<T> &c3,
           coordinate4<T> &c4)
        : coordinate1(c1), coordinate2(c2), coordinate3(c3), coordinate4(c4),
    {
        
    }
};

int main()
{
    float4 ns(1.0f, 2.0f, 3.0f, 1.0f);
    float4 ns2(1.0f, 2.0f, 3.0f, 1.0f);
    ns = ns * ns2;

    return 0;
}