#ifndef CAMERA
#define CAMERA


#define LOGER
#define XYLMATH

#include "Variables.hpp"
#include"Tools.hpp"
#include"Loger.hpp"
#include "Ray.hpp"
#include "Material.hpp"
#include "Constants.hpp"
//Image:
//确定图像像素大小:
//宽:
constexpr unsigned int IMG_WIDTH = 1028;
//高:
constexpr unsigned int IMG_HEIGHT = IMG_WIDTH / ratio_d_8_5;

constexpr int sample_per_pixel = 10;

constexpr double pixel_sample_scale = 1.0/sample_per_pixel;

// constexpr double fov = 90;

// vec3 lookfrom = point3(0,0,0);
// vec3 lookup = point3(0,1,0);
// vec3 lookat = point3(0,0,-1);


// #if defined(_GLIBCXX_FSTREAM) && defined(_GLIBCXX_CSTDINT)
class Camera{


    private:
        //Camera:
        vec3 lookfrom = point3(0,0,0);
        vec3 lookup = point3(0,1,0);
        vec3 lookat = point3(0,0,-1);
        
        double fov = 90;

        vec3 u,v,w;


        //Depth
        int maxDepth = 10;

        //相机位置
        point3 camera_center;



        //用来根据相机,设置场景位置(0,0,-1)
        double focal_length;

        double theta;
        double h;


        //viewport的目的是将屏幕空间转换到笛卡尔坐标空间
        double viewport_height;   
        double viewport_width;

        //viewport
        vec3 viewport_u;     //3.2,0,0
        vec3 viewport_v;     //0,-2,0

        //计算笛卡尔坐标系啊没个像素在view区间的所占比,最终会通过pixel_delta_u[0,viewport_u]乘以IMG_WIDTH或者IMG_HEIGHT来表示当前像素,处于viewport的什么位置
        vec3 pixel_delta_u; //3.2 / 1080 --> 0.00296296
        vec3 pixel_delta_v; //2 /657 --> -0.00474074

        //确定左上角像素在viewport的笛卡尔坐标,
        vec3 viewport_upper_left;//0,0,0 --> -1.6,1,-1

        vec3 pixel00_loc;//-1.6,1,-1 --> -1.59852,0.9985,-1




        //颜色计算
        color3 ray_color(const Ray& _ray , const hittable& world , int depth)const;
        vec3 sample_square(void)const;
        Ray get_ray(const int& a,const int& b)const;
        

        public:
            Camera();
            Camera(vec3 lookfrom , vec3 lookup , vec3 lookat , double fov);
            //渲染
            void RenderOld(std::ofstream& stream ,const hittable_list& world)const;
            void Render(std::ofstream& stream ,const hittable_list& world)const;

            void Initialize();

    };





#endif