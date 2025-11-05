#include <iostream>
#include <fstream>
#include <cstdint>

#define XYLMATH
#define RTONEWEEK
#define LOGER

#include "Variables.hpp"
#include "Tools.hpp"
#include "Camera.hpp"

// //绘制
// color3 ray_color(const ray& r , const hittable& world);

// void Render(std::ofstream& stream ,const Camera& camera,const hittable_list& world);

//主程序入口:
int main()
{
    //创建文件流:
    std::ofstream file("ColorImageAntialiasing.ppm" , std::ios::binary|std::ios::out);
    
    if(!file.is_open())
    {
        LOG_INFO("");
        return -1;
    }

    file<<"P6\n"<<IMG_WIDTH<<" "<<(int)IMG_HEIGHT<<"\n255\n";
    

    //Camera:
    Camera camera;
    

    //World:
    hittable_list world;

    world.add(std::make_shared<Sphere>(point3(0,0,-1),0.5));
    world.add(std::make_shared<Sphere>(point3(0,-100.5,-1),100));

    //开始渲染,写入文件:
    camera.Render(file,world);

    //关闭所有文件流:
    CloseStreamings(LOGER_FILE_STREAM,file);

    return 0;
} 

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

