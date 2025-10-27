#ifndef Variables
#define Variables

#include <iostream>
#include <cmath>
#include <fstream>

#include <vector>
#include <memory>

#include "Tools.hpp"

// using uint = unsigned int;

const constexpr auto ratio_d_16_9 = 16.0 / 9.0;

const constexpr auto ratio_d_8_5 = 8.0 / 5.0;

struct Color_255RGB
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    Color_255RGB() {};
    Color_255RGB(unsigned char _r, unsigned char _g, unsigned char _b);
};
struct Color_255RGB_double
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    Color_255RGB_double() {};
    Color_255RGB_double(double _r, double _g, double _b)
    {
        r = (unsigned char)_r;
        g = (unsigned char)_g;
        b = (unsigned char)_b;
    }
};
using Col_255RGB_d = Color_255RGB_double;

struct UV
{
public:
    float u = 0;
    float v = 0;
    UV();
    UV(float u, float v);
};

struct IMAGEPROPERY
{
public:
    UV uv;

    IMAGEPROPERY() {};
    IMAGEPROPERY(int width, int height);
};
using Img = IMAGEPROPERY;

class FX_LINEARFUNCTION_INT
{
    FX_LINEARFUNCTION_INT();

public:
    FX_LINEARFUNCTION_INT(float k, float a, float b, int width, int height);
    IMAGEPROPERY *img;
    int fx = 0;
    int f = 0;
    float k = 0;
    float a = 0;
    float b = 0;
    float x = 0;
};

enum class VEC_CHECKER : char
{
    e_vec = 1,
    e_col = 2
};

class vec3
{
private:
    double e[3];

public:
    const double *v = e;
    vec3() : e{0, 0, 0} {}

    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    const double x() const { return e[0]; } // 使返回的值不能被修改
    const double y() const { return e[1]; }
    const double z() const { return e[2]; }

    // 使用-号时,如果现在有vec3 direction变量,-direction可以使其返回相反数,但因为是const不会改变e所以只能接收.
    vec3 operator-() const
    {
        return vec3(-e[0], -e[1], -e[2]);
    }

    double operator[](int i) const
    {
        if (i < 0 || i > 3)
        {
            throw std::out_of_range("vec3 : the index out of the range");
        }
        return e[i];
    }

    double &operator[](int i)
    {
        if (i < 0 || i > 3)
        {
            throw std::out_of_range("vec3 : the index out of the range");
        }
        return e[i];
    }

    vec3 &operator+=(const vec3 &v)
    {
        e[0] += v.x();
        e[1] += v.y();
        e[2] += v.z();
        return *this;
    }

    vec3 &operator*=(double num)
    {
        e[0] *= num;
        e[1] *= num;
        e[1] *= num;
        return *this;
    }

    vec3 &operator/=(double num)
    {
        e[0] /= num;
        e[1] /= num;
        e[2] /= num;
        return *this;
    }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }
};

inline static std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    // 通过将传入的out进行类型转换,如果转换成功则为文件输出流,否则为标准输出流
    std::ofstream *ofs = dynamic_cast<std::ofstream *>(&out);

    if (ofs != nullptr)
    {
#ifdef _WIN32
        return out << (unsigned char)(int)v.x() << (unsigned char)(int)v.y() << (unsigned char)(int)v.z();
#endif
    }
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

inline vec3 operator*(const vec3 &v1, double num)
{
    return vec3(v1.x() * num, v1.y() * num, v1.z() * num);
}

inline vec3 operator*(const double num, const vec3 &v)
{
    return vec3(v.x() * num, v.y() * num, v.z() * num);
}

inline vec3 operator/(const vec3 &v1, double num)
{
    if (num == 0)
    {
        num = 0.000001;
    }
    double div = 1 / num;
    return vec3(v1.x() * div, v1.y() * div, v1.z() * div);
}
inline double dot(const vec3 &v1, const vec3 &v2)
{
    return ((v1.x() * v2.x()) + (v1.y() * v2.y()) + (v1.z() * v2.z()));
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.y() * v2.z() - v1.z() * v2.y(),
                v1.y() * v2.x() - v1.x() * v2.y(),
                v1.x() * v2.y() - v1.y() * v1.x());
}
inline vec3 normalize(const vec3 &v1, const VEC_CHECKER vecchacker)
{
    vec3 _v1 = v1;
    if (vecchacker == VEC_CHECKER::e_vec)
    {
        return _v1 / _v1.length();
    }
    else if (vecchacker == VEC_CHECKER::e_col)
    {
        return vec3(255, 100, 155);
    }
    return vec3(0, 0, 0);
}
inline vec3 color(const vec3 &v)
{
    return vec3(v.x() * 255, v.y() * 255, v.z() * 255);
}

// namespace VEC = VECTOR;
// using POINTS = VEC::vec3;
using point3 = vec3;
// using vec3 = VEC::vec3;
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
        // if(dir.length() > 1)
        // {
        //     dir = normalize(dir,VEC_CHECKER::e_vec);
        // }
        return origin + num * dir;
    }
};

using color3 = vec3;
inline void DrawUVImg(const float width, const float height, std::ofstream &file)
{
    vec3 color = vec3(0, 0, 0);
    for (int v = height; v > 0; v--)
    {
        for (int u = 0; u < width; u++)
        {
            double r = ((double)u / width * 255);
            double g = ((double)v / height * 255);
            double b = 0;

            color = color3(r, g, b);
            file << color;
        }
    }
}

class hit_record
{
public:
    point3 point;
    vec3 normal;
    double t;

    bool front_face;
    void set_face_normal(const ray &r, const vec3 &outward_normal)
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

    hittable_list(std::shared_ptr<hittable> objects)
    {
    }

    void add(std::shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    bool hit(const ray &r, const double ray_tmin, const double ray_max, hit_record &hitInfo) const override
    {
        hit_record temp;

        bool hit_anything = false;

        double closeset_so_far = ray_max;

        for (const hittable_list &object : objects)
        {
        }
    }
};

class Sphere : public hittable
{
private:
    point3 center;
    double radius;

public:
    Sphere(const point3 &c, const double &r) : center(c), radius(xyl::math::max(0.0, r)) {}
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
            root = (h + discriminant) / a;

            if (root <= ray_tmin || root >= ray_tmax)
                return false;
        }

        hitInfo.t = root;
        hitInfo.point = r.at(root);
        vec3 outward_normal = (hitInfo.point - center) / radius;
        hitInfo.set_face_normal(r, outward_normal);
    }
};

#endif // Variables