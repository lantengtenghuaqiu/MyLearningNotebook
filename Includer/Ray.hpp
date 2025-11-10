#ifndef RAY_H
#define RAY_H

#include <vector>
#include "Variables.hpp"
// #include "Metal.hpp"


class Ray
{
private:
    point3 origin;
    vec3 dir;

public:
    Ray() {}
    Ray(const point3 &orig, const vec3 &direction);
    const point3 &origination() const;
    const vec3 &Direction() const;
    point3 at(double num) const;

};

class Material;

class hit_record
{
public:
    point3 point;
    vec3 normal;
    std::shared_ptr<Material> mat;
    double t;

    bool front_face;

    void set_face_normal(const Ray &r, vec3 outward_normal);

};



class hittable
{
private:
public:
    virtual ~hittable() = default;
    virtual bool hit(const Ray &r, double ray_tmin, double ray_tmax, hit_record &hitInfo) const = 0;
};

class hittable_list : public hittable
{
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(std::shared_ptr<hittable> object);


    void add(std::shared_ptr<hittable> object);


    void clear();

    bool hit(const Ray &r, const double ray_tmin, const double ray_tmax, hit_record &hitInfo) const override;

};

class Sphere : public hittable
{
private:
    point3 center;
    double radius;
    std::shared_ptr<Material> mat;

public:
    Sphere(const point3 &center, const double &radius , std::shared_ptr<Material> mat);
    bool hit(const Ray &r, const double ray_tmin, const double ray_tmax, hit_record &hitInfo) const override;

};
#endif 
