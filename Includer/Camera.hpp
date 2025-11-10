#ifndef CAMERA
#define CAMERA


#define LOGER
#define XYLMATH

#include "Variables.hpp"
#include"Tools.hpp"
#include"Loger.hpp"
#include "Ray.hpp"
#include "Material.hpp"
//Image:
//确定图像像素大小:
//宽:
constexpr unsigned int IMG_WIDTH = 1028;
//高:
constexpr unsigned int IMG_HEIGHT = IMG_WIDTH / ratio_d_8_5;

constexpr int sample_per_pixel = 10;

constexpr double pixel_sample_scale = 1.0/sample_per_pixel;

// #if defined(_GLIBCXX_FSTREAM) && defined(_GLIBCXX_CSTDINT)
class Camera{
    private:
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

        //Depth
        int maxDepth = 10;

        //颜色计算
        color3 ray_color(const Ray& _ray , const hittable& world , int depth)const;

        Ray get_ray(const int& a,const int& b)const;
        vec3 sample_square(void)const;
        
    public:
        //渲染
        void RenderOld(std::ofstream& stream ,const hittable_list& world)const;
        void Render(std::ofstream& stream ,const hittable_list& world)const;


    };





#endif