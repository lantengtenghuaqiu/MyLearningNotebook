#include "Ray.hpp"

Ray::Ray(const point3 &orig, const vec3 &Direction): origin(orig), dir(Direction) {}

const point3 &Ray::origination() const { return origin; }

const vec3 &Ray::Direction() const { return dir; }

point3 Ray::at(double num) const
{
    return origin + num * dir;
}

void hit_record::set_face_normal(const Ray &r, vec3 outward_normal)
{
    front_face = dot(r.Direction(), outward_normal) < 0;

    normal = front_face ? outward_normal : -outward_normal;
}

hittable_list::hittable_list(std::shared_ptr<hittable> object)
{
    add(object);
}

void hittable_list::add(std::shared_ptr<hittable> object)
{
    objects.push_back(object);
}

void hittable_list::clear() { objects.clear(); }

bool hittable_list::hit(const Ray &r, const double ray_tmin, const double ray_tmax, hit_record &hitInfo) const
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

Sphere::Sphere(const point3 &center, const double &radius, std::shared_ptr<Material> mat) : center(center) , radius(xyl::math::max(0.0,radius)),mat(mat){}

bool Sphere::hit(const Ray &r, const double ray_tmin, const double ray_tmax, hit_record &hitInfo) const
{
    vec3 oc = center - r.origination();
    double a = dot(r.Direction(), r.Direction());
    double h = dot(r.Direction(), oc);
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
    hitInfo.mat = this->mat;
    vec3 outward_normal = (hitInfo.point - center) / radius;
    hitInfo.set_face_normal(r, outward_normal);

    return true;
}
