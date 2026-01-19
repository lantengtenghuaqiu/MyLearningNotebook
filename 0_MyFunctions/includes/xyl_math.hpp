#ifndef XYL_MATH
#define XYL_MATH

namespace xyl
{
    template <typename T>
    class coordinate4
    {
    protected:
        T coordinate[4] = {};

    public:
        T &operator[](int i)
        {
            if (i >= 0 && i <= 3)
                return this->coordinate[i];
            else
                return this->coordinate[0];
        }
    };

    class float4 : public coordinate4<float>
    {
    public:
        float4() {};
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

    class mat4x4{
        coordinate4 coordinate1;
        coordinate4 coordinate2;
        coordinate4 coordinate3;
        coordinate4 coordinate4;
        
        mat4x4(){}
        mat4x4(float4 coordinate1, float4 coordinate2, float4 coordinate3, float4 coordinate4)
        {
            this->coordinate1 = coordinate1;
            this->coordinate2 = coordinate2;
            this->coordinate3 = coordinate3;
            this->coordinate4 = coordinate4;
        };
    };

}

#endif