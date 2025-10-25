#include <iostream>
#include <fstream>

#include "Variables.hpp"
#include "Tools.hpp"

#include <cstdint>


const uint16_t IMG_WIDTH = 516;
const uint16_t IMG_HEIGHT = (IMG_WIDTH / ratio_d_8_5)<1? 1:(IMG_WIDTH / ratio_d_8_5);


color3 ray_color(const ray& r);
color3 DrawSphere(const ray& r,const vec3& center , const double radius);

int main()
{
    std::cout<<"Hello World"<<std::endl;

    std::ofstream file("ColorImage.ppm" , std::ios::binary|std::ios::out);

    if(!file.is_open())
    {
        LOG_INFO("");
        return -1;
    }

    file<<"P6\n"<<IMG_WIDTH<<" "<<(int)IMG_HEIGHT<<"\n255\n";

     double focal_legth = 1.0;
     double viewport_height = 2.0;
     double viewport_width = viewport_height * (static_cast<double>(IMG_WIDTH) / IMG_HEIGHT);
    


    const point3 camera_center = point3(0,0,0);
    vec3 viewport_u = vec3(viewport_width , 0 , 0);//3.2,0,0
    vec3 viewport_v = vec3(0 , -viewport_height , 0);//0,-2,0


    vec3 pixel_delta_u = viewport_u / IMG_WIDTH;//3.2 / 1080 --> 0.00296296
    vec3 pixel_delta_v = viewport_v / IMG_HEIGHT;//2 /657 --> -0.00474074


    vec3 viewport_upper_left = camera_center - vec3(0,0,focal_legth) - viewport_u/2 - viewport_v/2;//0,0,0 --> -1.6,1,-1


    vec3 pixel00_loc = viewport_upper_left +( 0.5 * (pixel_delta_u + pixel_delta_v));//-1.6,1,-1 --> -1.59852,0.9985,-1
    // std::cout<<pixel00_loc<<std::endl;

    // DrawUVImg(IMG_WIDTH,IMG_HEIGHT,file);
    for(int v = 0 ; v < IMG_HEIGHT ; v++)
    {
        for(int u = 0 ; u <IMG_WIDTH ; u++)
        {
                                //u = 0 : v = 0;        pixel_center = (-1.59852,0.9985,-1);
                                //u = 1080 : v = 657;   pixel_center = (-1.59852,0.9985,-1) + (3.2,0,0) + (0,-2,0);
            vec3 pixel_center = pixel00_loc + (pixel_delta_u * u) + (pixel_delta_v * v);
            vec3 ray_direction = pixel_center - camera_center; 
            ray r(camera_center,ray_direction);
            // std::cout<<ray_direction<<std::endl;
            // txt<<ray_direction;
            color3 pixel_color = DrawSphere(r,camera_center,0.5) * 255;

            file<<pixel_color;
        }
    }
    LOG_INFO(camera_center);
    


    file.close();



    return 0;
} 

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

double hit_sphere(const ray& r ,const vec3& center , const double radius)
{
    vec3 OC = center - r.origination();
    double a = dot(r.direction(),r.direction());
    double b = -2.0 *  dot(r.direction(),OC);
    double c = dot(OC,OC) - radius*radius;
    double discriminant = b*b - 4*a*c;
    // std::cout<<discriminant<<std::endl;
    // V3_LOGE<double>(discriminant,0.0,0.0);
   if(discriminant < 0)
   {
        return -1.0;
   }
   else
   {
        return (- b - std::sqrt(discriminant)) / (2.0 * a);
   }
    // return (discriminant >= 0);
}

// color3 ray_color(const ray& r)
// {   
//     vec3 unit_direction = normalize(r.direction(),VEC_CHECKER::e_vec);
//     auto a = 0.5*(unit_direction.y() + 1.0);
//     return (1.0-a)*color3(1.0, 1.0, 1.0) + a*color3(0.5, 0.7, 1.0);
// }

color3 DrawSphere(const ray& r,const vec3& center , const double radius)
{
    
    double t = hit_sphere(r,point3(0,0,-1),radius);
    if(t > 0.0)
    {
        vec3 N = normalize(r.at(t)-vec3(0,0,-1),VEC_CHECKER::e_vec);
        return 0.5*color3(N.x()+1, N.y()+1, N.z()+1);
    }


    vec3 unit_direction = normalize(r.direction(),VEC_CHECKER::e_vec);
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color3(1.0, 1.0, 1.0) + a*color3(0.5, 0.7, 1.0);
}
