#include <iostream>
#include <fstream>
#include <cstdint>

#define LOGER
#define XYLMATH
#include "Tools.hpp"

#include "Variables.hpp"

//Image:
//确定图像像素大小:
//宽:
constexpr const uint16_t IMG_WIDTH = 516;
//高:
constexpr const uint16_t IMG_HEIGHT = (static_cast<double>(IMG_WIDTH) / ratio_d_8_5)<1? 1: static_cast<uint16_t>(static_cast<double>(IMG_WIDTH) / ratio_d_8_5);

//绘制
color3 ray_color(const ray& r , const hittable& world);



//主程序入口:
int main()
{
    std::cout<<"Hello World"<<std::endl;
    //创建文件流:
    std::ofstream file("ColorImage.ppm" , std::ios::binary|std::ios::out);
    
    if(!file.is_open())
    {
        LOG_INFO("");
        return -1;
    }

    file<<"P6\n"<<IMG_WIDTH<<" "<<(int)IMG_HEIGHT<<"\n255\n";

    //World:
     hittable_list world;

     world.add(std::make_shared<Sphere>(point3(0,0,-1),0.5));
     world.add(std::make_shared<Sphere>(point3(0,-100.5,-1),100));

    //Camera:
    //相机位置
    const point3 camera_center = point3(0,0,0);
    //用来根据相机,设置场景位置(0,0,-1)
    double focal_legth = 1.0;
    //viewport的目的是将屏幕空间转换到笛卡尔坐标空间
    double viewport_height = 2.0;   
    double viewport_width = viewport_height * (static_cast<double>(IMG_WIDTH) / IMG_HEIGHT);
    
    //viewport
    vec3 viewport_u = vec3(viewport_width , 0 , 0);//3.2,0,0
    vec3 viewport_v = vec3(0 , -viewport_height , 0);//0,-2,0

    //计算笛卡尔坐标系啊没个像素在view区间的所占比,最终会通过pixel_delta_u[0,viewport_u]乘以IMG_WIDTH或者IMG_HEIGHT来表示当前像素,处于viewport的什么位置
    vec3 pixel_delta_u = viewport_u / IMG_WIDTH;//3.2 / 1080 --> 0.00296296
    vec3 pixel_delta_v = viewport_v / IMG_HEIGHT;//2 /657 --> -0.00474074

    //确定左上角像素在viewport的笛卡尔坐标,
    vec3 viewport_upper_left = camera_center - vec3(0,0,focal_legth) - viewport_u/2 - viewport_v/2;//0,0,0 --> -1.6,1,-1

    vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);//-1.6,1,-1 --> -1.59852,0.9985,-1

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
            color3 pixel_color = ray_color(r,world) * 255;

            file<<pixel_color;
        }
    }
    // LOG_INFO(camera_center);
    


    CloseStreamings(LOGER_FILE_STREAM,file);

    return 0;
} 

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

color3 ray_color(const ray& r , const hittable& world)
{
    hit_record hitInfo;
    if(world.hit(r,0,infinity ,hitInfo))
    {
        return 0.5 * (hitInfo.normal + color3(1,1,1)); 
    }

    vec3 unitDir = normalize(r.direction(),VEC_CHECKER::e_vec); 

    double a = 0.5 * (unitDir.y() + 1.0);

    return (1.0 - a) * color3(1.0,1.0,1.0) + a * color3(0.5, 0.7, 1.0);

}