#ifndef Variables
#define Variables

#include <iostream>
#include <cmath>
#include <fstream>

#include <vector>
#include <memory>

#define XYLMATH

#include "Tools.hpp"
#include "Constants.hpp"

// using uint = unsigned int;

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
    vec3 operator-();
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


    

};

vec3 RandomUnitVector();

vec3 RandomOnHemisphere(const vec3& normal);

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
    }
}

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


using point3 = vec3;

class ray
{
private:
    point3 origin;
    vec3 dir;

public:
    ray() {}
    ray(const point3 &orig, const vec3 &direction) : origin(orig), dir(direction) {}
    const point3 &origination() const { return origin; }
    const vec3 &direction() const { return dir; }
    point3 at(double num) const
    {
        return origin + num * dir;
    }
};

using color3 = vec3;

class hit_record
{
public:
    point3 point;
    vec3 normal;
    double t;

    bool front_face;

    void set_face_normal(const ray &r, vec3 outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;

        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
private:
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &hitInfo) const = 0;
};

class hittable_list : public hittable
{
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(std::shared_ptr<hittable> object)
    {
        add(object);
    }

    void add(std::shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    void clear() { objects.clear(); }

    bool hit(const ray &r, const double ray_tmin, const double ray_tmax, hit_record &hitInfo) const override
    {
        hit_record temp_hitInfo;

        bool hit_anything = false;

        double closeset_so_far = ray_tmax;

        for (const std::shared_ptr<hittable> &object : objects)
        {
            if (object->hit(r, ray_tmin, closeset_so_far, temp_hitInfo))
            {
                hit_anything = true;
                closeset_so_far = temp_hitInfo.t;

                hitInfo = temp_hitInfo;
            }
        }
        return hit_anything;
    }
};

class Sphere : public hittable
{
private:
    point3 center;
    double radius;

public:
    Sphere(const point3 &c, const double &r) : center(c), radius(std::fmax(0.0, r)) {}
    bool hit(const ray &r, const double ray_tmin, const double ray_tmax, hit_record &hitInfo) const override
    {
        vec3 oc = center - r.origination();
        double a = dot(r.direction(), r.direction());
        double h = dot(r.direction(), oc);
        double c = dot(oc, oc) - xyl::math::pow(radius, 2);

        double discriminant = xyl::math::pow(h, 2) - a * c;

        if (discriminant < 0)
        {
            return false;
        }

        double sqrtd = std::sqrt(discriminant);

        double root = (h - sqrtd) / a;

        if (root <= ray_tmin || root >= ray_tmax)
        {
            root = (h + sqrtd) / a;

            if (root <= ray_tmin || root >= ray_tmax)
                return false;
        }

        hitInfo.t = root;
        hitInfo.point = r.at(hitInfo.t);
        vec3 outward_normal = (hitInfo.point - center) / radius;
        hitInfo.set_face_normal(r, outward_normal);

        return true;
    }
};

namespace xyl
{
    class Interval
    {
    private:
        double min, max;

    public:
        Interval() : min(-xyl::consts::infinity), max(+xyl::consts::infinity) {}
    };
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