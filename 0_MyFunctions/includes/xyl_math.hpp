#ifndef XYL_MATH
#define XYL_MATH

#include <stdio.h>
#include <cmath>

#define pi M_PI
#define rad pi / 180.0
namespace xyl
{
    /*
    template <typename T>
    class Coordinate4
    {

    public:
        union
        {
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };
            // __m128 simd;
            T vec[4];
        };

        Coordinate4() : x(0), y(0), z(0), w(0) {};
        Coordinate4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {};
        ~Coordinate4() {}
        Coordinate4(const Coordinate4 &other) : x(other.x), y(other.y), z(other.z), w(other.w)
        {
            // this->x = other.x;
            // this->y = other.y;
            // this->z = other.z;
            // this->w = other.w;
        }

        Coordinate4 &operator=(const Coordinate4 &other)
        {
            if (this == &other)
                return *this;
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->w = other.w;
            return *this;
        }

        T &operator[](int i)
        {
            switch (i)
            {
            case 0:
                return this->x;
            case 1:
                return this->y;
            case 2:
                return this->z;
            case 3:
                return this->w;
            default:
                return this->x;
            }
        }
        const T &operator[](int i) const
        {
            switch (i)
            {
            case 0:
                return this->x;
            case 1:
                return this->y;
            case 2:
                return this->z;
            case 3:
                return this->w;
            default:
                return this->x;
            }
        }
        Coordinate4 operator*(const Coordinate4 &other) const
        {
            Coordinate4 temp;
            temp.x = this->x * other.x;
            temp.y = this->y * other.y;
            temp.z = this->z * other.z;
            temp.w = this->w * other.w;
            return temp;
        }

        Coordinate4 operator*(const T &sclae) const
        {
            Coordinate4 temp;
            temp.x = this->x * sclae;
            temp.y = this->y * sclae;
            temp.z = this->z * sclae;
            temp.w = this->w * sclae;
            return temp;
        }

        Coordinate4 operator/(const Coordinate4 &other) const
        {
            Coordinate4 temp;
            temp.x = this->x / other.x;
            temp.y = this->y / other.y;
            temp.z = this->z / other.z;
            temp.w = this->w / other.w;
            return temp;
        }

        Coordinate4 operator/(const T &scale) const
        {
            Coordinate4 temp;
            temp.x = this->x / scale;
            temp.y = this->y / scale;
            temp.z = this->z / scale;
            temp.w = this->w / scale;
            return temp;
        }

        Coordinate4 operator+(const Coordinate4 &other) const
        {
            Coordinate4 temp;
            temp.x = this->x + other.x;
            temp.y = this->y + other.y;
            temp.z = this->z + other.z;
            temp.w = this->w + other.w;
            return temp;
        }

        Coordinate4 operator-(const Coordinate4 &other) const
        {
            Coordinate4 temp;
            temp.x = this->x - other.x;
            temp.y = this->y - other.y;
            temp.z = this->z - other.z;
            temp.w = this->w - other.w;
            return temp;
        }

        // T operator+=(){
        //     return
        // }
    };

    using float4 = Coordinate4<float>;

    template <typename T>
    class _mat4x4
    {
    public:
        xyl::Coordinate4<T> row1;
        xyl::Coordinate4<T> row2;
        xyl::Coordinate4<T> row3;
        xyl::Coordinate4<T> row4;

        _mat4x4()
        {
        }

        _mat4x4(xyl::Coordinate4<T> row1,
                xyl::Coordinate4<T> row2,
                xyl::Coordinate4<T> row3,
                xyl::Coordinate4<T> row4)
        {
            this->row1 = row1;
            this->row2 = row2;
            this->row3 = row3;
            this->row4 = row4;

        }
        _mat4x4(
            T m11, T m12, T m13, T m14,
            T m21, T m22, T m23, T m24,
            T m31, T m32, T m33, T m34,
            T m41, T m42, T m43, T m44)
        {
            this->row1.x = m11;
            this->row1.y = m12;
            this->row1.z = m13;
            this->row1.w = m14;

            this->row2.x = m21;
            this->row2.y = m22;
            this->row2.z = m23;
            this->row2.w = m24;

            this->row3.x = m31;
            this->row3.y = m32;
            this->row3.z = m33;
            this->row3.w = m34;

            this->row4.x = m41;
            this->row4.y = m42;
            this->row4.z = m43;
            this->row4.w = m44;
        }

        xyl::Coordinate4<T> operator*(const xyl::Coordinate4<T> &other)
        {
            T x = this->row1.x * other.x + this->row1.y * other.y + this->row1.z * other.z + this->row1.w * other.w;
            T y = this->row2.x * other.x + this->row2.y * other.y + this->row2.z * other.z + this->row2.w * other.w;
            T z = this->row3.x * other.x + this->row3.y * other.y + this->row3.z * other.z + this->row3.w * other.w;
            T w = this->row4.x * other.x + this->row4.y * other.y + this->row4.z * other.z + this->row4.w * other.w;

            xyl::Coordinate4<T> multiple(x, y, z, w);

            return multiple;
        }

        xyl::Coordinate4<T> &operator[](int i)
        {
            switch (i)
            {
            case 0:
                return this->row1;

            case 1:
                return this->row2;

            case 2:
                return this->row3;

            case 3:
                return this->row4;

            default:

                return this->row1;
            }
        }

        const xyl::Coordinate4<T> &operator[](int i) const
        {
            switch (i)
            {
            case 0:
                return this->row1;

            case 1:
                return this->row2;

            case 2:
                return this->row3;

            case 3:
                return this->row4;

            default:
                return this->row1;
            }
        }
    };

    template <typename T>
    xyl::_mat4x4<T> operator*(const xyl::_mat4x4<T> &a, const xyl::_mat4x4<T> &b)
    {
        xyl::_mat4x4<T> temp;
        for (int m = 0; m < 4; m++)
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    temp[m][i] += a[m][j] * b[j][i];
                }
            }
        }
        return temp;
    }

    static xyl::_mat4x4<float> Identity(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0);

    static xyl::_mat4x4<float> Zero(
        0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0);

    namespace transform
    {
        xyl::_mat4x4<float> Scale(float scale = 0.0f)
        {
            xyl::_mat4x4<float> scaleMatrix(
                scale, 0.0f, 0.0f, 0.0f,
                0.0f, scale, 0.0f, 0.0f,
                0.0f, 0.0f, scale, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            return scaleMatrix;
        }

// @Default calculation of the trigonometric function uses the radian value.So, the rotation transform matrix needs a conversion from radian to degree.

    xyl::_mat4x4<float> Rotation(float angleX = 0.0f, float angleY = 0.0f, float angleZ = 0.0f)
    {
        xyl::_mat4x4<float> rx(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cos(angleX * rad), -sin(angleX * rad), 0.0f,
            0.0f, sin(angleX * rad), cos(angleX * rad), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        xyl::_mat4x4<float> ry(
            cos(angleY * rad), 0.0f, sin(angleY * rad), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sin(angleY * rad), 0.0f, cos(angleY * rad), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        xyl::_mat4x4<float> rz(
            cos(angleZ * rad), -sin(angleZ * rad), 0.0f, 0.0f,
            sin(angleZ * rad), cos(angleZ * rad), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        return rz * ry * rx;
    }

    xyl::_mat4x4<float> Translate(float tx = 0.0, float ty = 0.0, float tz = 0.0)
    {
        xyl::_mat4x4<float> translateMatrix(
            1.0f, 0.0f, 0.0f, tx,
            0.0f, 1.0f, 0.0f, ty,
            0.0f, 0.0f, 1.0f, tz,
            0.0f, 0.0f, 0.0f, 1.0f);

        return translateMatrix;
    }

    xyl::_mat4x4<float> M(float S, float AngleX, float AngleY, float AngleZ, float Tx, float Ty, float Tz)
    {
        xyl::_mat4x4<float> modelToWorldMatrix;
        modelToWorldMatrix = Translate(Tx, Ty, Tz) * Rotation(AngleX, AngleY, AngleZ) * Scale(S);

        return modelToWorldMatrix;
    }
    template <typename T>
    void ConsoleMatrix(xyl::_mat4x4<T> m)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%f \t", m[i][j]);
            }

            printf("\n");
        }
    }
} // namespace transform
*/
    template<typename T>
    struct vec{
        
    };

    template <typename T>
    class mat4x4
    {
    public:
        T matrix[16] = {0};
        mat4x4() {}
        mat4x4(const T *numbers)
        {
            for (int i = 0; i < 16; i++)
            {

                this->matrix[i] = numbers[i];
            }
        }
        mat4x4(
            T m00, T m01, T m02, T m03,
            T m10, T m11, T m12, T m13,
            T m20, T m21, T m22, T m23,
            T m30, T m31, T m32, T m33)
        {
            this->matrix[0] = m00;
            this->matrix[1] = m01;
            this->matrix[2] = m02;
            this->matrix[3] = m03;

            this->matrix[4] = m10;
            this->matrix[5] = m11;
            this->matrix[6] = m12;
            this->matrix[7] = m13;

            this->matrix[8] = m20;
            this->matrix[9] = m21;
            this->matrix[10] = m22;
            this->matrix[11] = m23;

            this->matrix[12] = m30;
            this->matrix[13] = m31;
            this->matrix[14] = m32;
            this->matrix[15] = m33;
        }

        T operator*(const mat4x4<T> &other) const
        {
            xyl::mat4x4<T> temp;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    temp.matrix[i * 4 + j] *= other.matrix[i + j * 4];
                }
            }
        }
    };
}

namespace xyl
{
    namespace math
    {
        // template <typename T>
        // static T Dot(xyl::Coordinate4<T> a, xyl::Coordinate4<T> b)
        // {
        //     return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
        // }
    }
} // namespace xyl

#endif