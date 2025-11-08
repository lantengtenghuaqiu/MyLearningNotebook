#include <iostream>
#include <fstream>
#include <cstdint>


#define XYLMATH
#define RTONEWEEK
#define LOGER

#define Chapter4

#include "Camera.hpp"
#include "Variables.hpp"
#include "Tools.hpp"
#include "Loger.hpp"

//���������:
int main()
{
    //�����ļ���:
    std::ofstream file("ColorImageAntialiasing.ppm" , std::ios::binary|std::ios::out);

    if(!file.is_open())
    {
        LOG_INFO("");
        return -1;
    }

    file<<"P6\n"<<IMG_WIDTH<<" "<<IMG_HEIGHT<<"\n255\n";

    //Camera:
    Camera camera;

    //World:
    hittable_list world;

    world.add(std::make_shared<Sphere>(point3(0,0,-1),0.5));
    world.add(std::make_shared<Sphere>(point3(0,-100.5,-1),100));

    //��ʼ��Ⱦ,д���ļ�:
    camera.Render(file,world);

    //�ر������ļ���:
    xyl::log::CloseStreamings(xyl::log::LOGER_FILE_STREAM,file);

    return 0;
} 

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------