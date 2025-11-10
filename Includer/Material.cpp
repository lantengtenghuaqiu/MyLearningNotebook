#include "Material.hpp"

Lambertain::Lambertain(){}

Lambertain::Lambertain(const color3 &albedo) : albedo(albedo) {}

bool Lambertain::scatter(const Ray &ray_in, const hit_record &hitInfo, color3 &attenuation, Ray &scattered) const
{
    vec3 scatter_direction = hitInfo.normal + RandomUnitVector();

    if(scatter_direction.NearZero())
        scatter_direction = hitInfo.normal;

    scattered = Ray(hitInfo.point , scatter_direction);

    attenuation = albedo;

    return true;
}

Metal::Metal(){}

Metal::Metal(const color3 &albedo , const double& fuzz):albedo(albedo) , fuzz(fuzz<1.0?fuzz:1.0){}

bool Metal::scatter(const Ray &ray_in, const hit_record &hitInfo, color3 &attenuation, Ray &scattered) const
{
    vec3 reflection = xyl::vec::Reflect(ray_in.Direction() , hitInfo.normal);
    reflection = normalize(reflection,VEC_CHECKER::e_vec) + (this->fuzz * RandomUnitVector());

    scattered = Ray(hitInfo.point , reflection);

    attenuation = albedo;

    return (dot(scattered.Direction() , hitInfo.normal)>0.0);
}

Dielectic::Dielectic(double refraction_index) : refraction_index(refraction_index){}

bool Dielectic::scatter(const Ray &ray_in, const hit_record &hitInfo, color3 &attenuation, Ray &scattered) const
{
    attenuation = color3(1.0,1.0,1.0);

    double ri = hitInfo.front_face ? (1.0/refraction_index) : refraction_index;

    vec3 unit_diretion = normalize(ray_in.Direction(),VEC_CHECKER::e_vec);
    
    vec3 refracted = xyl::vec::Refract(unit_diretion , hitInfo.normal , ri);

    scattered = Ray(hitInfo.point , refracted);

    return true;
}
