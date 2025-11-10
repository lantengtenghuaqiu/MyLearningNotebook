#include <iostream>
#include <fstream>
#include <cstdint>

#define LOGER
#define XYLMATH
#include "Tools.hpp"

#include "Variables.hpp"


//确定图像像素大小:
//宽:
constexpr const uint16_t IMG_WIDTH = 216;
//高:
constexpr const uint16_t IMG_HEIGHT = (static_cast<double>(IMG_WIDTH) / ratio_d_8_5)<1? 1: static_cast<uint16_t>(static_cast<double>(IMG_WIDTH) / ratio_d_8_5);

//绘制圆函数
color3 DrawSphere(const Ray& r,const vec3& center , const double radius);



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
    //用来根据相机,设置场景位置(0,0,-1)
    double focal_legth = 1.0;
    //viewport的目的是将屏幕空间转换到笛卡尔坐标空间
    double viewport_height = 2.0;   
    double viewport_width = viewport_height * (static_cast<double>(IMG_WIDTH) / IMG_HEIGHT);
    
    const point3 camera_center = point3(0,0,0);
    vec3 viewport_u = vec3(viewport_width , 0 , 0);//3.2,0,0
    vec3 viewport_v = vec3(0 , -viewport_height , 0);//0,-2,0
    //计算笛卡尔坐标系啊没个像素在view区间的所占比,最终会通过pixel_delta_u[0,viewport_u]乘以IMG_WIDTH或者IMG_HEIGHT来表示当前像素,处于viewport的什么位置
    vec3 pixel_delta_u = viewport_u / IMG_WIDTH;//3.2 / 1080 --> 0.00296296
    vec3 pixel_delta_v = viewport_v / IMG_HEIGHT;//2 /657 --> -0.00474074

    //确定左上角像素在viewport的笛卡尔坐标,
    vec3 viewport_upper_left = camera_center - vec3(0,0,focal_legth) - viewport_u/2 - viewport_v/2;//0,0,0 --> -1.6,1,-1

    vec3 pixel00_loc = viewport_upper_left +( 0.5 * (pixel_delta_u + pixel_delta_v));//-1.6,1,-1 --> -1.59852,0.9985,-1

    for(int v = 0 ; v < IMG_HEIGHT ; v++)
    {
        for(int u = 0 ; u <IMG_WIDTH ; u++)
        {
                                //u = 0 : v = 0;        pixel_center = (-1.59852,0.9985,-1);
                                //u = 1080 : v = 657;   pixel_center = (-1.59852,0.9985,-1) + (3.2,0,0) + (0,-2,0);
            vec3 pixel_center = pixel00_loc + (pixel_delta_u * u) + (pixel_delta_v * v);
            vec3 ray_direction = pixel_center - camera_center; 
            Ray r(camera_center,ray_direction);
            // std::cout<<ray_direction<<std::endl;
            // txt<<ray_direction;
            color3 pixel_color = DrawSphere(r,camera_center,0.5) * 255;

            file<<pixel_color;
        }
    }
    // LOG_INFO(camera_center);
    


    CloseStreamings(LOGER_FILE_STREAM,file);

    return 0;
} 

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//通过delta x求射线与圆的交集,射线穿过sphere的公式,delta > 0会返回两个值,delta x = 0相交返回一个,delta x < 0不相交
double ray_color(const Ray& r ,const vec3& center , const double radius)
{
    //算法优化前
    {
        // vec3 OC = center - r.origination();
        // double a = dot(r.direction(),r.direction());
        // double b = -2.0 *  dot(r.direction(),OC);
        // double c = dot(OC,OC) - xyl::math::pow(radius,2);
        // double discriminant = xyl::math::pow(b,2) - 4*a*c;
        // if(discriminant < 0)
        // {
        //         return -1.0;
        // }
        // else
        // {
        //         return (- b - std::sqrt(discriminant)) / (2.0 * a);
        // }
    }

    //算法优化后
    vec3 OC = center - r.origination();
    double a = dot(r.direction(),r.direction());
    double h = dot(r.direction(),OC);
    double c = dot(OC,OC) - xyl::math::pow(radius,2);
    double discriminant = xyl::math::pow(h,2) - a*c;


   if(discriminant < 0)
   {
        return -1.0;
   }
   else
   {
        return (h - std::sqrt(discriminant)) / a;
   }
}


color3 DrawSphere(const Ray& r,const vec3& center , const double radius)
{
    //绘制Sphere的
    double t = ray_color(r,point3(0,0,-1),radius);
    // DOLOGE({t});
    if(t > 0.0)
    {
        vec3 N = normalize(r.at(t)-vec3(0,0,-1),VEC_CHECKER::e_vec);
        DOLOGE({N});
        
        return 0.5*color3(N.x()+1, N.y()+1, N.z()+1);
    }

    //绘制背景的
    vec3 unit_direction = normalize(r.direction(),VEC_CHECKER::e_vec);
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color3(1.0, 1.0, 1.0) + a*color3(0.5, 0.7, 1.0);
}


