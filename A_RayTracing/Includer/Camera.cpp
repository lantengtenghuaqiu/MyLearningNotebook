#include "Camera.hpp"

color3 Camera::ray_color(const Ray& _ray, const hittable& world , int depth) const
{
    hit_record hitRecord;
    color3 result;
    vec3 unitDir;
    double a=0;

    if(depth <= 0)
    {
        return color3(0,0,0);
    }

#ifdef ANTIALIASING
    // const
    if (world.hit(_ray, 0, xyl::consts::infinity, hitRecord))
    {
        result = 0.5 * (hitRecord.normal + color3(1, 1, 1));
        return result;
    }

    vec3 unitDir = normalize(_ray.Direction(), VEC_CHECKER::e_vec);

    double a = 0.5 * (unitDir.y() + 1.0);

    return (1.0 - a) * color3(1.0, 1.0, 1.0) + a * color3(0.5, 0.7, 1.0);
#endif

#ifdef DIFFUSE
    // const
    double min = 0.001;
    double absorb  = 0.9;
    if (world.hit(_ray, min, xyl::consts::infinity, hitRecord))
    {
        result = hitRecord.normal + RandomUnitVector();
        return absorb * Camera::ray_color(ray(hitRecord.point,result),world , depth-1);
    }

    unitDir = normalize(_ray.Direction(), VEC_CHECKER::e_vec);

    a = 0.5 * (unitDir.y() + 1.0);

    return (1.0 - a) * color3(1.0, 1.0, 1.0) + a * color3(0.5, 0.7, 1.0);
#endif

#ifdef MATERIAL
    if(world.hit(_ray,0.001,xyl::consts::infinity , hitRecord))
    {
        Ray scattered;
        color3 attenuation;
        if(hitRecord.mat->scatter(_ray,hitRecord,attenuation,scattered))
            return attenuation * ray_color(scattered,world,depth-1);

        return color3(0,0,0);
    }

    unitDir = normalize(_ray.Direction(), VEC_CHECKER::e_vec);

    a = 0.5 * (unitDir.y() + 1.0);

    return (1.0 - a) * color3(1.0, 1.0, 1.0) + a * color3(0.5, 0.7, 1.0);
#endif

    std::cerr<<"None definition at here : "<<__LINE__<<std::endl;
    exit(0);
}


void Camera::RenderOld(std::ofstream &stream, const hittable_list &world) const
{
    for (unsigned int v = 0; v < IMG_HEIGHT; v++)
    {
        for (unsigned int u = 0; u < IMG_WIDTH; u++)
        {
            // u = 0 : v = 0;        pixel_center = (-1.59852,0.9985,-1);
            // u = 1080 : v = 657;   pixel_center = (-1.59852,0.9985,-1) + (3.2,0,0) + (0,-2,0);
            vec3 pixel_center = this->pixel00_loc + (this->pixel_delta_u * u) + (this->pixel_delta_v * v);
            vec3 ray_direction = pixel_center - this->camera_center;
            Ray r(this->camera_center, ray_direction);

            vec3 rayColor = xyl::vec::clamp<vec3, double>(ray_color(r, world , this->maxDepth), 0.0, 1.0);

            color3 pixel_color = rayColor * 255;

            stream << pixel_color;
        }
    }
}

Ray Camera::get_ray(const int &v, const int &u) const
{
    vec3 offset = sample_square();
    vec3 pixel_sample = pixel00_loc + ((u + offset.x()) * this->pixel_delta_u) + ((offset.y() + v) * this->pixel_delta_v);
    vec3 ray_origin = this->camera_center;
    vec3 ray_direction = pixel_sample - ray_origin;
    return Ray(ray_origin, ray_direction);
}

vec3 Camera::sample_square(void) const
{
    return vec3(xyl::math::random_double_normalized() - 0.5, xyl::math::random_double_normalized() - 0.5, 0.0);
}

void Camera::Render(std::ofstream &stream, const hittable_list &world) const
{
    unsigned int r;
    unsigned int g;
    unsigned int b;

    color3 result(0, 0, 0);
    color3 pixel_color(0, 0, 0);

    for (unsigned int v = 0; v < IMG_HEIGHT; v++)
    {
        for (unsigned int u = 0; u < IMG_WIDTH; u++)
        {
            pixel_color(0.0, 0.0, 0.0);
            for (int sample = 0; sample < sample_per_pixel; sample++)
            {
                Ray r = get_ray(v, u);
                pixel_color += ray_color(r, world ,this->maxDepth);
                // xyl::log::DOLOGE({sample});
            }
            
            result = pixel_color * pixel_sample_scale ;
            
            r = xyl::Render::linear_to_gamma(result.v[0])* 255;
            g = xyl::Render::linear_to_gamma(result.v[1])* 255;
            b = xyl::Render::linear_to_gamma(result.v[2])* 255;
            
            // xyl::log::DOLOGE({this->lookat});
            // stream<<result;
            stream.write((const char *)(&r), 1);
            stream.write((const char *)(&g), 1);
            stream.write((const char *)(&b), 1);
        }
    }
}



Camera::Camera()
{
    this->lookfrom  = point3(0,0,0);
    this->lookup    = point3(0,1,0);
    this->lookat    = point3(0,0,-1);

    this->fov = 90;

}

Camera::Camera(vec3 lookfrom, vec3 lookup, vec3 lookat, double fov)
{
    this->lookfrom = lookfrom;
    this->lookup = lookup;
    this->lookat = lookat;

    this->fov = fov;

}

void Camera::Initialize()
{


    this->camera_center = lookfrom;
    this->focal_length = (lookfrom - lookat).length();
    this->theta = xyl::math::DegToRad(fov);
    this->h = std::tan(theta/2.0);

    this->viewport_height = 2.0 * h * focal_length;
    this->viewport_width = viewport_height * (static_cast<double>(IMG_WIDTH) / IMG_HEIGHT);

    this->w = normalize(lookfrom - lookat,VEC_CHECKER::e_vec);
    this->u = normalize(cross(lookup , w),VEC_CHECKER::e_vec);
    this->v = cross(w,u);

    this->viewport_u = viewport_width * u;  
    this->viewport_v = viewport_height * -v;

    this->pixel_delta_u  = viewport_u / IMG_WIDTH;   
    this->pixel_delta_v= viewport_v / IMG_HEIGHT;  

    this->viewport_upper_left = camera_center - (focal_length * w) - viewport_u/2 - viewport_v/2;
    this->pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}
