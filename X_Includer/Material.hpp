#ifndef METAL_H
#define METAL_H

#include "Ray.hpp"


class Material
{
    public:
        virtual ~Material() = default;

        virtual bool scatter(const Ray& ray_in ,  const hit_record& hitInfo , color3& attenuation , Ray& scattered)const
        {
            return false;
        }
};

class Lambertain : public Material
{
    private:
        color3 albedo;
        Lambertain();
    public:

        Lambertain(const color3& albedo);

        bool scatter(const Ray& ray_in ,  const hit_record& hitInfo , color3& attenuation , Ray& scattered)const override;
};

class Metal : public Material
{
    private:
        color3 albedo;
        double fuzz;
        Metal();
    public:
        Metal(const color3& albedo , const double& fuzz);

        bool scatter(const Ray& ray_in ,  const hit_record& hitInfo , color3& attenuation , Ray& scattered)const override;
};

class Dielectic : public Material
{
    private:
        Dielectic();
        double refraction_index;
    public:
        Dielectic(double refraction_index);
        bool scatter(const Ray& ray_in ,  const hit_record& hitInfo , color3& attenuation , Ray& scattered)const override;
    static double reflectance(const double& cosine , const double& refraction_index);
};
#endif