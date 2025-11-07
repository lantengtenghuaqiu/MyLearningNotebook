#include"Camera.hpp"


color3 Camera::ray_color(const ray &_ray, const hittable &world) const
{
            hit_record hitInfo;
            color3 result;
            // const 
            if(world.hit(_ray,0,xyl::consts::infinity ,hitInfo))
            {   
                result = 0.5 * (hitInfo.normal + color3(1,1,1));
                return result; 
            }

            vec3 unitDir = normalize(_ray.direction(),VEC_CHECKER::e_vec); 

            double a = 0.5 * (unitDir.y() + 1.0);
            

            return (1.0 - a) * color3(1.0,1.0,1.0) + a * color3(0.5, 0.7, 1.0);
}

void Camera::RenderOld(std::ofstream &stream, const hittable_list &world) const
{  
    for(unsigned int v = 0 ; v < IMG_HEIGHT ; v++)
    {
        for(unsigned int u = 0 ; u <IMG_WIDTH ; u++)
        {
                                //u = 0 : v = 0;        pixel_center = (-1.59852,0.9985,-1);
                                //u = 1080 : v = 657;   pixel_center = (-1.59852,0.9985,-1) + (3.2,0,0) + (0,-2,0);
            vec3 pixel_center = this->pixel00_loc + (this->pixel_delta_u * u) + (this->pixel_delta_v * v);
            vec3 ray_direction = pixel_center - this->camera_center; 
            ray r(this->camera_center,ray_direction);

            vec3 rayColor = xyl::vec::clamp<vec3,double>(ray_color(r,world),0.0,1.0);

            color3 pixel_color = rayColor * 255;

            stream<<pixel_color;
        }
    }
}


ray Camera::get_ray(const int& v,const int& u) const
{
    vec3 offset = sample_square();
    vec3 pixel_sample = pixel00_loc + ((u + offset.x()) * this->pixel_delta_u) + ((offset.y() + v) * this->pixel_delta_v);
    vec3 ray_origin = this->camera_center;
    vec3 ray_direction = pixel_sample - ray_origin;
    return ray(ray_origin , ray_direction);
}

vec3 Camera::sample_square(void) const
{
    return vec3(xyl::math::random_double_normalized() - 0.5, xyl::math::random_double_normalized() - 0.5 ,0.0);
}


void Camera::Render(std::ofstream &stream, const hittable_list &world) const
{
    
    for(unsigned int v = 0; v<IMG_HEIGHT ; v++)
    {
        for(unsigned int u = 0; u < IMG_WIDTH ; u++)
        {
            color3 pixel_color(0.0,0.0,0.0);
            for(int sample = 0 ; sample < sample_per_pixel;sample++)
            {
                ray r = get_ray(v,u);
                pixel_color += ray_color(r,world);
            }
            color3 result = pixel_color * pixel_sample_scale*255;
            xyl::log::DOLOGE({result});
            stream<<result;
        }
    }
}
