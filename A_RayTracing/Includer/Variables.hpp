#ifndef Variables
#define Variables

// #include <iostream>

#include <memory>

#define XYLMATH

#include "Tools.hpp"

constexpr float ratio_d_16_9 = 16.0 / 9.0;
constexpr float ratio_d_8_5 = 8.0 / 5.0;

enum class VEC_CHECKER : char
{
    e_vec,
    e_col,
    e_ppm,
    e_file
};

class vec3;
double dot(const vec3 &v1, const vec3 &v2);


class vec3
{
private:
    double e[3];
    VEC_CHECKER checker = VEC_CHECKER::e_vec;
public:
    const double* v = e;
    vec3();

    vec3(const double& e0, const double& e1,const double& e2);
    vec3(const double& e0, const double& e1,const double& e2 ,const VEC_CHECKER& check);

    double x() const; // Return value can not be returned
    double y() const;
    double z() const;
    VEC_CHECKER GetChecker()const;
    // 使用-号时,如果现在有vec3 direction变量,-direction可以使其返回相反数,但因为是const不会改变e所以只能接收.
    vec3 operator-()const;
    // vec3 operator+(double i)const;

    double operator[](int i) const;

    double &operator[](int i);

    vec3 &operator+=(const vec3 &v);

    vec3 &operator()(const double& e0 , const double& e1 , const double& e2);


    vec3 &operator*=(double num);

    vec3 &operator/=(double num);


    double DotVec3() const;


    double length() const;

    static vec3 random();

    template<typename T>
    static vec3 random(const T& min , const T& max)
    {
        return vec3(xyl::math::random_range<double>(min , max),xyl::math::random_range<double>(min , max),xyl::math::random_range<double>(min , max));
    }

    bool NearZero()const;
    

};

using point3 = vec3;
using color3 = vec3;

vec3 RandomUnitVector();

vec3 RandomOnHemisphere(const vec3& normal);



std::ostream &operator<<(std::ostream &out, const vec3 &v);

vec3 operator+(const vec3 &v1, const vec3 &v2);


vec3 operator-(const vec3 &v1, const vec3 &v2);

vec3 operator*(const vec3 &v1, const vec3 &v2);

vec3 operator*(const vec3 &v1, double num);

vec3 operator*(const double num, const vec3 &v);


vec3 operator/(const vec3 &v1, double num);


vec3 cross(const vec3 &v1, const vec3 &v2);


vec3 normalize(const vec3 &v1, const VEC_CHECKER vecchacker);


vec3 color(const vec3 &v);


namespace xyl
{
    namespace vec
    {
        template <typename K, typename T>
        inline K clamp(const K &val, const T &min, const T &max)
        {
            if constexpr (xyl::types::is_same<K, vec3>::value)
            {
                float x = val.x() < min ? min : val.x() > max ? max
                                                              : val.x();
                float y = val.y() < min ? min : val.y() > max ? max
                                                              : val.y();
                float z = val.z() < min ? min : val.z() > max ? max
                                                              : val.z();
                return vec3(x, y, z);
            }
            return val;
        }

        template<typename K >
        inline K Reflect(const K& v , const K& n)
        {
            return v - 2*dot(v,n)*n;
        }


        inline vec3 Refract(const vec3& uv , const vec3& n , const double& etai_over_etat)
        {
            double cos_theta = xyl::math::min(dot(-uv,n),1.0);

            vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);

            vec3 r_out_parallel = -std::sqrt(xyl::math::abs(1.0 - r_out_perp.length())) * n;

            return r_out_perp + r_out_parallel;
        }
    }
}



// UV color example
// namespace xyl
// {
//     namespace example
//     {
//         inline void DrawUVImg(const unsigned int width, const unsigned int height, std::ofstream &file)
//         {
//             vec3 color = vec3(0, 0, 0);
//             for (unsigned int v = height; v > 0; v--)
//             {
//                 for (unsigned int u = 0; u < width; u++)
//                 {
//                     double r = ((double)u / width * 255);
//                     double g = ((double)v / height * 255);
//                     double b = 0;

//                     color = color3(r, g, b);
//                     file << color;
//                 }
//             }
//         }
//     }
// } // namespace xyl

#endif // Variables