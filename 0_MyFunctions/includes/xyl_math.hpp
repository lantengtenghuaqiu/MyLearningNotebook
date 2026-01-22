#ifndef XYL_MATH
#define XYL_MATH

namespace xyl
{
    template <typename T>
    class coordinate4
    {
    protected:
        T coord[4] = {};

    public:
        T x;
        T y;
        T z;
        T w;
        coordinate4()
        {
            this->x = this->coord[0];
            this->y = this->coord[1];
            this->z = this->coord[2];
            this->w = this->coord[3];
        }
        coordinate4(const coordinate4 &other)//主动实现拷贝构造函数,则赋值消失
        {
            this->coord[0] = other.coord[0];
            this->coord[1] = other.coord[1];
            this->coord[2] = other.coord[2];
            this->coord[3] = other.coord[3];
            
            this->x = other.coord[0];
            this->y = other.coord[1];
            this->z = other.coord[2];
            this->w = other.coord[3];
        }
        T &operator[](int i)
        {
            if (i >= 0 && i <= 3)
                return this->coord[i];
            else
                return this->coord[0];
        }
    };

    class float4 : public xyl::coordinate4<float>
    {
    public:
        float4() {};
        float4(float x, float y, float z, float w)
        {
            this->coord[0] = x;
            this->coord[1] = y;
            this->coord[2] = z;
            this->coord[3] = w;
        };
        ~float4() {}
        float4(const float4 &other)
        {
            this->coord[0] = other.coord[0];
            this->coord[1] = other.coord[1];
            this->coord[2] = other.coord[2];
            this->coord[3] = other.coord[3];
        }
        float4 operator*(const float4 &other) const
        {
            float4 temp;
            temp.coord[0] = this->coord[0] * other.coord[0];
            temp.coord[1] = this->coord[1] * other.coord[1];
            temp.coord[2] = this->coord[2] * other.coord[2];
            temp.coord[3] = this->coord[3] * other.coord[3];
            return temp;
        }
        float4 operator/(const float4 &other) const
        {
            float4 temp;
            temp.coord[0] = this->coord[0] / other.coord[0];
            temp.coord[1] = this->coord[1] / other.coord[1];
            temp.coord[2] = this->coord[2] / other.coord[2];
            temp.coord[3] = this->coord[3] / other.coord[3];
            return temp;
        }
        float4 operator+(const float4 &other) const
        {
            float4 temp;
            temp.coord[0] = this->coord[0] + other.coord[0];
            temp.coord[1] = this->coord[1] + other.coord[1];
            temp.coord[2] = this->coord[2] + other.coord[2];
            temp.coord[3] = this->coord[3] + other.coord[3];
            return temp;
        }

        float4 operator-(const float4 &other) const
        {
            float4 temp;
            temp.coord[0] = this->coord[0] - other.coord[0];
            temp.coord[1] = this->coord[1] - other.coord[1];
            temp.coord[2] = this->coord[2] - other.coord[2];
            temp.coord[3] = this->coord[3] - other.coord[3];
            return temp;
        }
        float4 &operator=(const float4 &other)
        {
            if (this == &other)
                return *this;
            this->coord[0] = other.coord[0];
            this->coord[1] = other.coord[1];
            this->coord[2] = other.coord[2];
            this->coord[3] = other.coord[3];
            return *this;
        }
    };

    template <typename T>
    class mat4x4
    {
    public:
        xyl::coordinate4<T> coord1;
        xyl::coordinate4<T> coord2;
        xyl::coordinate4<T> coord3;
        xyl::coordinate4<T> coord4;

        mat4x4() {}

        mat4x4(xyl::coordinate4<T> coord1,
               xyl::coordinate4<T> coord2,
               xyl::coordinate4<T> coord3,
               xyl::coordinate4<T> v)
        {
            this->coord1 = coord1;
            this->coord2 = coord2;
            this->coord3 = coord3;
            this->coord4 = coord4;
        };
    };

}

#endif