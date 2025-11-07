#include <iostream>
#include <fstream>
#include <cstdint>

#define XYLMATH
#define RTONEWEEK
#define LOGER

#include "Camera.hpp"
#include "Variables.hpp"
#include "Tools.hpp"


//主程序入口:
int main()
{
    //创建文件流:
    std::ofstream file("ColorImageAntialiasing2.ppm" , std::ios::binary|std::ios::out);
    
    if(!file.is_open())
    {
        LOG_INFO("");
        return -1;
    }

    file<<"P6\n"<<IMG_WIDTH<<" "<<(int)IMG_HEIGHT<<"\n255\n";


    //Camera:
    Camera camera;
    
    xyl::strlen("xxx",3);
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

